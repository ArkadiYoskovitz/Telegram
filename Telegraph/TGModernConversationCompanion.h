/*
 * This is the source code of Telegram for iOS v. 1.1
 * It is licensed under GNU GPL v. 2 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Peter Iakovlev, 2013.
 */

#import <Foundation/Foundation.h>

#import <SSignalKit/SSignalKit.h>

#import <LegacyComponents/ASWatcher.h>

#import "TGMessageRange.h"

#import "TGApplicationFeatures.h"

#import "TGVisibleMessageHole.h"

@class TGModernConversationController;
@class TGModernViewContext;
@class TGMessage;
@class TGMessageModernConversationItem;
@class TGVideoMediaAttachment;
@class TGDocumentMediaAttachment;
@class TGUser;
@class TGVenueAttachment;
@class TGGameMediaAttachment;
@class TGPreparedMessage;

@class TGModernConversationEmptyListPlaceholderView;
@class TGModernConversationInputPanel;
@class TGModernViewInlineMediaContext;
@class TGLiveUploadActorData;
@class TGDataItem;

@class TGBingSearchResultItem;
@class TGGiphySearchResultItem;
@class TGWebSearchInternalImageResult;
@class TGWebSearchInternalGifResult;
@class TGExternalGifSearchResult;
@class TGInternalGifSearchResult;

@class TGICloudItem;
@class TGDropboxItem;

@class TGBotContextResultAttachment;
@class TGImageMediaAttachment;
@class TGExternalImageSearchResult;

@class TGMediaAsset;
@class TGVideoEditAdjustments;

@class TGAudioWaveform;

@class TGMessageEditingContext;
@class TGBotContextResult;

@class TGBotReplyMarkup;

@class TGModernGalleryController;

@class TLInputMedia;

typedef enum {
    TGInitialScrollPositionTop = 0,
    TGInitialScrollPositionCenter = 1,
    TGInitialScrollPositionBottom = 2
} TGInitialScrollPosition;

typedef enum {
    TGModernConversationAddMessageIntentGeneric = 0,
    TGModernConversationAddMessageIntentSendTextMessage = 1,
    TGModernConversationAddMessageIntentSendOtherMessage = 2,
    TGModernConversationAddMessageIntentLoadMoreMessagesAbove = 3,
    TGModernConversationAddMessageIntentLoadMoreMessagesBelow = 4
} TGModernConversationAddMessageIntent;

typedef enum {
    TGModernConversationControllerTitleToggleNone,
    TGModernConversationControllerTitleToggleShowDiscussion,
    TGModernConversationControllerTitleToggleHideDiscussion
} TGModernConversationControllerTitleToggle;

typedef enum {
    TGMessageModerateActionDelete,
    TGMessageModerateActionBan,
    TGMessageModerateActionReport,
    TGMessageModerateActionDeleteAll
} TGMessageModerateAction;

@interface TGModernConversationCompanion : NSObject <ASWatcher>
{
    NSArray *_items;
    NSMutableDictionary *_uploadingEditMessages;
}

@property (nonatomic, strong) ASHandle *actionHandle;

@property (nonatomic, weak) TGModernConversationController *controller;
@property (nonatomic, strong) TGModernViewContext *viewContext;

@property (nonatomic) TGMessageIndex *focusedOnMessageIndex;
@property (nonatomic) TGMessageIndex *mediaHiddenMessageIndex;

@property (nonatomic) bool previewMode;
@property (nonatomic) bool useInitialSnapshot;

@property (nonatomic, strong) SVariable *callbackInProgress;

+ (void)warmupResources;

+ (bool)isMessageQueue;
+ (void)dispatchOnMessageQueue:(dispatch_block_t)block;
+ (SQueue *)messageQueue;
- (void)lockSendMessageSemaphore;
- (void)unlockSendMessageSemaphore;

- (void)setInitialMessagePositioning:(int32_t)initialPositionedMessageId initialPositionedPeerId:(int64_t)initialPositionedPeerId position:(TGInitialScrollPosition)position offset:(CGFloat)offset;
- (void)setUnreadMessageRange:(TGMessageRange)unreadMessageRange;
- (TGMessageRange)unreadMessageRange;
- (int32_t)initialPositioningMessageId;
- (int64_t)initialPositioningPeerId;
- (TGInitialScrollPosition)initialPositioningScrollPosition;
- (CGFloat)initialPositioningScrollOffset;
- (CGFloat)initialPositioningOverflowForScrollPosition:(TGInitialScrollPosition)scrollPosition;

- (void)bindController:(TGModernConversationController *)controller;
- (void)unbindController;
- (void)loadInitialState;
- (void)subscribeToUpdates;

- (NSString *)title;
- (void)_setTitle:(NSString *)title;
- (void)_setAvatarConversationId:(int64_t)conversationId title:(NSString *)title icon:(UIImage *)icon;
- (void)_setAvatarConversationIds:(NSArray *)conversationIds titles:(NSArray *)titles;
- (void)_setAvatarConversationId:(int64_t)conversationId firstName:(NSString *)firstName lastName:(NSString *)lastName;
- (void)_setTitleIcons:(NSArray *)titleIcons;
- (void)_setAvatarUrl:(NSString *)avatarUrl;
- (void)_setAvatarUrls:(NSArray *)avatarUrls;
- (void)_setStatus:(NSString *)status accentColored:(bool)accentColored allowAnimation:(bool)allowAnimation toggleMode:(TGModernConversationControllerTitleToggle)toggleMode;
- (void)_setTitle:(NSString *)title andStatus:(NSString *)status accentColored:(bool)accentColored allowAnimatioon:(bool)allowAnimation toggleMode:(TGModernConversationControllerTitleToggle)toggleMode;
- (void)_setTypingStatus:(NSString *)typingStatus activity:(int)activity;

- (void)_controllerWillAppearAnimated:(bool)animated firstTime:(bool)firstTime;
- (void)_controllerDidAppear:(bool)firstTime;
- (void)_controllerAvatarPressed;
- (void)_dismissController;
- (void)_setControllerWidthForItemCalculation:(CGFloat)width;
- (void)_loadControllerPrimaryTitlePanel;
- (TGModernConversationEmptyListPlaceholderView *)_conversationEmptyListPlaceholder;
- (TGModernConversationInputPanel *)_conversationGenericInputPanel;
- (TGModernConversationInputPanel *)_conversationEmptyListInputPanel;
- (void)_updateInputPanel;
- (UIView *)_conversationHeader;
- (UIView *)_controllerInputTextPanelAccessoryView;
- (void)updateControllerInputText:(NSString *)inputText entities:(NSArray *)entities messageEditingContext:(TGMessageEditingContext *)messageEditingContext;
- (void)controllerDidUpdateTypingActivity;
- (void)controllerDidCancelTypingActivity;
- (void)controllerDidChangeInputText:(NSString *)inputText;
- (void)controllerWantsToSendTextMessage:(NSString *)text entities:(NSArray *)entities asReplyToMessageId:(int32_t)replyMessageId withAttachedMessages:(NSArray *)withAttachedMessages completeGroups:(NSSet *)completeGroups disableLinkPreviews:(bool)disableLinkPreviews botContextResult:(TGBotContextResultAttachment *)botContextResult botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToSendMapWithLatitude:(double)latitude longitude:(double)longitude venue:(TGVenueAttachment *)venue period:(int32_t)period asReplyToMessageId:(int32_t)replyMessageId botContextResult:(TGBotContextResultAttachment *)botContextResult botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (NSURL *)fileUrlForDocumentMedia:(TGDocumentMediaAttachment *)documentMedia;
- (NSDictionary *)imageDescriptionFromImage:(UIImage *)image stickers:(NSArray *)stickers caption:(NSString *)caption entities:(NSArray *)entities optionalAssetUrl:(NSString *)assetUrl allowRemoteCache:(bool)allowRemoteCache timer:(int32_t)timer;
- (NSDictionary *)imageDescriptionFromBingSearchResult:(TGBingSearchResultItem *)item caption:(NSString *)caption entities:(NSArray *)entities;
- (NSDictionary *)imageDescriptionFromExternalImageSearchResult:(TGExternalImageSearchResult *)item text:(NSString *)text entities:(NSArray *)entities botContextResult:(TGBotContextResultAttachment *)botContextResult;
- (NSDictionary *)documentDescriptionFromGiphySearchResult:(TGGiphySearchResultItem *)item caption:(NSString *)caption entities:(NSArray *)entities;
- (NSDictionary *)documentDescriptionFromExternalGifSearchResult:(TGExternalGifSearchResult *)item text:(NSString *)text entities:(NSArray *)entities botContextResult:(TGBotContextResultAttachment *)botContextResult;
- (NSDictionary *)documentDescriptionFromBotContextResult:(TGBotContextResult *)result text:(NSString *)text entities:(NSArray *)entities botContextResult:(TGBotContextResultAttachment *)botContextResult;
- (NSDictionary *)imageDescriptionFromMediaAsset:(TGMediaAsset *)asset previewImage:(UIImage *)previewImage document:(bool)document fileName:(NSString *)fileName caption:(NSString *)caption entities:(NSArray *)entities allowRemoteCache:(bool)allowRemoteCache;
- (NSDictionary *)videoDescriptionFromMediaAsset:(TGMediaAsset *)asset previewImage:(UIImage *)previewImage dimensions:(CGSize)dimensions duration:(NSTimeInterval)duration adjustments:(TGVideoEditAdjustments *)adjustments document:(bool)document fileName:(NSString *)fileName stickers:(NSArray *)stickers caption:(NSString *)caption entities:(NSArray *)entities timer:(int32_t)timer;
- (NSDictionary *)videoDescriptionFromVideoURL:(NSURL *)videoURL previewImage:(UIImage *)previewImage dimensions:(CGSize)dimensions duration:(NSTimeInterval)duration adjustments:(TGVideoEditAdjustments *)adjustments stickers:(NSArray *)stickers caption:(NSString *)caption entities:(NSArray *)entities roundMessage:(bool)roundMessage liveUploadData:(id)liveUploadData timer:(int32_t)timer;
- (NSDictionary *)documentDescriptionFromICloudDriveItem:(TGICloudItem *)item;
- (NSDictionary *)documentDescriptionFromDropboxItem:(TGDropboxItem *)item;
- (NSDictionary *)imageDescriptionFromInternalSearchImageResult:(TGWebSearchInternalImageResult *)item caption:(NSString *)caption entities:(NSArray *)entities;
- (NSDictionary *)documentDescriptionFromInternalSearchResult:(TGWebSearchInternalGifResult *)item caption:(NSString *)caption entities:(NSArray *)entities;
- (NSDictionary *)documentDescriptionFromRemoteDocument:(TGDocumentMediaAttachment *)document caption:(NSString *)caption entities:(NSArray *)entities;
- (NSDictionary *)documentDescriptionFromFileAtTempUrl:(NSURL *)url fileName:(NSString *)fileName mimeType:(NSString *)mimeType isAnimation:(bool)isAnimation caption:(NSString *)caption entities:(NSArray *)entities;
- (void)controllerWantsToSendImagesWithDescriptions:(NSArray *)imageDescriptions asReplyToMessageId:(int32_t)replyMessageId botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToSendLocalVideoWithTempFilePath:(NSString *)tempVideoFilePath fileSize:(int32_t)fileSize previewImage:(UIImage *)previewImage duration:(NSTimeInterval)duration dimensions:(CGSize)dimenstions caption:(NSString *)caption entities:(NSArray *)entities assetUrl:(NSString *)assetUrl liveUploadData:(TGLiveUploadActorData *)liveUploadData asReplyToMessageId:(int32_t)replyMessageId botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (TGVideoMediaAttachment *)serverCachedAssetWithId:(NSString *)assetId;
- (void)controllerWantsToSendDocumentWithTempFileUrl:(NSURL *)tempFileUrl fileName:(NSString *)fileName mimeType:(NSString *)mimeType asReplyToMessageId:(int32_t)replyMessageId;
- (void)controllerWantsToSendDocumentsWithDescriptions:(NSArray *)descriptions asReplyToMessageId:(int32_t)replyMessageId;
- (void)controllerWantsToSendRemoteDocument:(TGDocumentMediaAttachment *)document asReplyToMessageId:(int32_t)replyMessageId text:(NSString *)text entities:(NSArray *)entities botContextResult:(TGBotContextResultAttachment *)botContextResult botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToSendRemoteImage:(TGImageMediaAttachment *)image text:(NSString *)text entities:(NSArray *)entities asReplyToMessageId:(int32_t)replyMessageId botContextResult:(TGBotContextResultAttachment *)botContextResult botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToSendCloudDocumentsWithDescriptions:(NSArray *)descriptions asReplyToMessageId:(int32_t)replyMessageId;
- (void)controllerWantsToSendLocalAudioWithDataItem:(TGDataItem *)dataItem duration:(NSTimeInterval)duration liveData:(TGLiveUploadActorData *)liveData waveform:(TGAudioWaveform *)waveform asReplyToMessageId:(int32_t)replyMessageId botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToSendRemoteVideoWithMedia:(TGVideoMediaAttachment *)media asReplyToMessageId:(int32_t)replyMessageId text:(NSString *)text entities:(NSArray *)entities botContextResult:(TGBotContextResultAttachment *)botContextResult botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToSendContact:(TGUser *)contactUser asReplyToMessageId:(int32_t)replyMessageId botContextResult:(TGBotContextResultAttachment *)botContextResult botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToSendGame:(TGGameMediaAttachment *)gameMedia asReplyToMessageId:(int32_t)replyMessageId botContextResult:(TGBotContextResultAttachment *)botContextResult botReplyMarkup:(TGBotReplyMarkup *)botReplyMarkup;
- (void)controllerWantsToResendMessages:(NSArray *)messageIds;
- (void)controllerWantsToForwardMessages:(NSArray *)messageIndices;
- (void)controllerWantsToCreateContact:(int32_t)uid firstName:(NSString *)firstName lastName:(NSString *)lastName phoneNumber:(NSString *)phoneNumber attachment:(TGContactMediaAttachment *)attachment;
- (void)controllerWantsToAddContactToExisting:(int32_t)uid phoneNumber:(NSString *)phoneNumber attachment:(TGContactMediaAttachment *)attachment;
- (void)controllerWantsToApplyLocalization:(NSString *)filePath;
- (void)controllerClearedConversation;
- (void)systemClearedConversation;
- (void)controllerDeletedMessages:(NSArray *)messageIds forEveryone:(bool)forEveryone completion:(void (^)())completion;
- (void)controllerCanReadHistoryUpdated;
- (void)controllerCanRegroupUnreadIncomingMessages;
- (void)controllerRequestedNavigationToConversationWithUser:(int32_t)uid;
- (bool)controllerShouldStoreCapturedAssets;
- (bool)controllerShouldCacheServerAssets;
- (bool)controllerShouldLiveUploadVideo;
- (bool)imageDownloadsShouldAutosavePhotos;
- (bool)shouldAutomaticallyDownloadPhotos;
- (bool)shouldAutomaticallyDownloadAnimations;
- (bool)shouldAutomaticallyDownloadVideoMessages;
- (bool)shouldAutomaticallyDownloadVideos;
- (bool)shouldAutomaticallyDownloadVideoOfSize:(int32_t)size;
- (bool)shouldAutomaticallyDownloadDocuments;
- (bool)shouldAutomaticallyDownloadDocumentOfSize:(int32_t)size;
- (bool)allowMessageForwarding;
- (bool)allowMessageExternalSharing;
- (bool)allowReplies;
- (bool)allowMessageEntities;
- (bool)allowCaptionEntities;
- (bool)allowExternalContent;
- (bool)allowContactSharing;
- (bool)allowVenueSharing;
- (bool)allowCaptionedMedia;
- (bool)allowCaptionedDocuments;
- (bool)allowVideoMessages;
- (bool)allowSelfDescructingMedia;
- (bool)allowLiveLocations;
- (bool)allowMediaGrouping;
- (bool)encryptUploads;
- (bool)canPostMessages;
- (bool)skipServiceMessages;
- (NSDictionary *)userActivityData;
- (TGApplicationFeaturePeerType)applicationFeaturePeerType;

- (void)updateControllerEmptyState:(bool)force;
- (void)clearCheckedMessages;
- (void)setMessageChecked:(TGMessageIndex *)messageIndex checked:(bool)checked;
- (NSInteger)checkedMessageCount;
- (NSArray *)checkedMessageIndices;
- (TGUser *)checkedMessageModerateUser;
- (bool)_isMessageChecked:(TGMessageIndex *)messageIndex;
- (bool)_isGroupChecked:(int64_t)groupedId;

- (void)_setMessageFlags:(int32_t)messageId flags:(int)flags;
- (void)_setMessageViewDate:(int32_t)messageId viewDate:(NSTimeInterval)viewDate;
- (void)_setMessageFlagsAndViewDate:(int32_t)messageId flags:(int)flags viewDate:(NSTimeInterval)viewDate;
- (bool)_isSecretMessageViewed:(int32_t)messageId;
- (bool)_isSecretMessageScreenshotted:(int32_t)messageId;
- (NSTimeInterval)_secretMessageViewDate:(int32_t)messageId;

- (void)updateMessagesLive:(NSDictionary *)messageIdToMessage animated:(bool)animated;
- (void)_updateMessageItemsWithData:(NSArray *)items;
- (void)_updateMediaStatusDataForCurrentItems;
- (void)_updateMediaStatusDataForItemsInIndexSet:(NSIndexSet *)indexSet animated:(bool)animated forceforceCheckDownload:(bool)forceCheckDownload;
- (void)_updateMediaStatusDataForItemsWithMessageIdsInSet:(NSMutableSet *)messageIds;
- (void)_downloadMediaInMessage:(TGMessage *)message highPriority:(bool)highPriority;
- (void)_updateProgressForItemsInIndexSet:(NSIndexSet *)indexSet animated:(bool)animated;
- (void)_resetProgressForItemsInIndexSet:(NSIndexSet *)indexSet;
- (TGMessageModernConversationItem *)_updateMediaStatusData:(TGMessageModernConversationItem *)item;
- (void)_updateImportantMediaStatusDataInplace:(TGMessageModernConversationItem *)item;

- (void)_itemsUpdated;

- (void)refreshItems:(void (^)(void))completion;

- (void)loadMoreMessagesAbove;
- (void)loadMoreMessagesBelow;
- (void)unloadMessagesAbove;
- (void)unloadMessagesBelow;

- (void)_performFastScrollDown:(bool)becauseOfSendTextAction becauseOfNavigation:(bool)becauseOfNavigation;
- (void)_replaceMessages:(NSArray *)newMessages;
- (void)_replaceMessages:(NSArray *)newMessages atMessageId:(int32_t)atMessageId peerId:(int64_t)peerId expandFrom:(int32_t)expandMessageId jump:(bool)jump top:(bool)top messageIdForVisibleHoleDirection:(int32_t)messageIdForVisibleHoleDirection scrollBackMessageId:(int32_t)scrollBackMessageId animated:(bool)animated;
- (void)_replaceMessagesWithFastScroll:(NSArray *)newMessages intent:(TGModernConversationAddMessageIntent)intent scrollToMessageId:(int32_t)scrollToMessageId peerId:(int64_t)peerId scrollBackMessageId:(int32_t)scrollBackMessageId animated:(bool)animated;
- (void)_addMessages:(NSArray *)addedMessages animated:(bool)animated intent:(TGModernConversationAddMessageIntent)intent;
- (void)_addMessages:(NSArray *)addedMessages animated:(bool)animated intent:(TGModernConversationAddMessageIntent)intent deletedMessages:(NSArray *)deletedMessages;
- (void)_addedMessages:(NSArray *)messages;
- (void)_deleteMessages:(NSArray *)messages animated:(bool)animated;
- (void)_updateMessageDelivered:(int32_t)previousMid;
- (void)_updateMessageDelivered:(int32_t)previousMid mid:(int32_t)mid date:(int32_t)date message:(TGMessage *)message pts:(int32_t)pts;
- (void)_updateMessageDeliveryFailed:(int32_t)previousMid;
- (void)_updateMessages:(NSDictionary *)messagesByIds;

- (void)updateMediaAccessTimeForMessageId:(int32_t)messageId;

- (id)acquireAudioRecordingActivityHolder;
- (id)acquireVideoMessageRecordingActivityHolder;
- (id)acquireLocationPickingActivityHolder;

- (void)serviceNotificationsForMessageIds:(NSArray *)messageIds;
- (void)markMessagesAsViewed:(NSArray *)messageIds;

- (SSignal *)userListForMention:(NSString *)mention canBeContextBot:(bool)canBeContextBot includeSelf:(bool)includeSelf;
- (SSignal *)inlineResultForMentionText:(NSString *)mention text:(NSString *)text;
- (SSignal *)hashtagListForHashtag:(NSString *)hashtag;
- (SSignal *)commandListForCommand:(NSString *)command;

- (void)navigateToMessageId:(int32_t)messageId scrollBackMessageId:(int32_t)scrollBackMessageId forceUnseenMention:(bool)forceUnseenMention animated:(bool)animated;

- (void)navigateToMessageSearch;

- (bool)isASingleBotGroup;
- (bool)suppressesOutgoingUnreadContents;

- (void)_controllerDidUpdateVisibleHoles:(NSArray *)holes;
- (void)_controllerDidUpdateVisibleUnseenMessageIds:(NSArray *)unseenMessageIds;
- (void)_controllerDidUpdateVisibleUnseenMentionMessageIds:(NSArray *)unseenMentionMessageIds;
- (bool)_controllerShouldHideInputTextByDefault;
- (bool)canDeleteMessage:(TGMessage *)message;
- (bool)canModerateMessage:(TGMessage *)message;
- (bool)canEditMessage:(TGMessage *)message;
- (bool)canPinMessage:(TGMessage *)message;
- (bool)canDeleteMessageForEveryone:(TGMessage *)message;
- (bool)isMessagePinned:(int32_t)messageId;
- (bool)canDeleteMessages;
- (bool)canDeleteAllMessages;

- (int64_t)requestPeerId;
- (int64_t)requestAccessHash;
- (int64_t)attachedPeerId;

- (void)_toggleBroadcastMode;

- (void)updateMessageViews:(NSDictionary *)messageIdToViews markAsSeen:(bool)markAsSeen;
- (void)_toggleTitleMode;

- (SSignal *)contextBotInfoForText:(NSString *)text;

- (SSignalQueue *)mediaUploadQueue;

- (id)playlistMetadata:(bool)voice;

- (void)maybeAskForSecretWebpages;
- (void)maybeAskForInlineBots;

- (SSignal *)editingContextForMessageWithId:(int32_t)messageId;
- (SSignal *)saveEditedMessageWithId:(int32_t)messageId text:(NSString *)text entities:(NSArray *)entities disableLinkPreviews:(bool)disableLinkPreviews;

- (SSignal *)updatePinnedMessage:(int32_t)messageId;

- (bool)canCreateLinksToMessages;

- (SSignal *)applyModerateMessageActions:(NSSet *)actions messageIds:(NSArray *)messageIds;

- (bool)canReportMessage:(TGMessage *)message;
- (void)reportMessageIndices:(NSArray *)messageIndices menuController:(TGMenuSheetController *)menuController;

- (TGModernGalleryController *)galleryControllerForAvatar;

- (bool)canAddNewMessagesToTop;

- (bool)isPeerAdmin;

- (void)startVoiceCall;
- (bool)supportsCalls;

- (bool)canAttachLinkPreviews;
- (NSNumber *)inlineMediaRestrictionTimeout;
- (NSNumber *)mediaRestrictionTimeout;
- (NSNumber *)stickerRestrictionTimeout;

- (bool)messageSearchByUserAvailable;
- (bool)messageSearchByDateAvailable;

- (SSignal *)alphacodeListForQuery:(NSString *)query;
- (void)performBotAutostart:(NSString *)param;

@end
