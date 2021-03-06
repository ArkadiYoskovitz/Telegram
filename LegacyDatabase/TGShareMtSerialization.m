#import "TGShareMtSerialization.h"

#import "ApiLayer82.h"

#import <MTProtoKitDynamic/MTExportedAuthorizationData.h>
#import <MTProtoKitDynamic/MTDatacenterAddress.h>

@implementation TGShareMtSerialization

- (NSUInteger)currentLayer
{
    return 82;
}

- (id)parseMessage:(NSData *)data
{
    return [Api82__Environment parseObject:data];
}

- (MTExportAuthorizationResponseParser)exportAuthorization:(int32_t)datacenterId data:(__autoreleasing NSData **)data
{
    Api82_FunctionContext *exportAuthorization = [Api82 auth_exportAuthorizationWithDcId:@(datacenterId)];
    
    if (data)
        *data = exportAuthorization.payload;
    
    return ^MTExportedAuthorizationData *(NSData *data) {
        id response = exportAuthorization.responseParser(data);
        if ([response isKindOfClass:[Api82_auth_ExportedAuthorization class]])
        {
            Api82_auth_ExportedAuthorization *exportedAuthorization = response;
            return [[MTExportedAuthorizationData alloc] initWithAuthorizationBytes:exportedAuthorization.bytes authorizationId:[exportedAuthorization.pid intValue]];
        }
        return nil;
    };
}

- (NSData *)importAuthorization:(int32_t)authId bytes:(NSData *)bytes
{
    Api82_FunctionContext *importAuthorization = [Api82 auth_importAuthorizationWithPid:@(authId) bytes:bytes];
    
    return importAuthorization.payload;
}

- (MTRequestDatacenterAddressListParser)requestDatacenterAddressWithData:(__autoreleasing NSData **)data
{
    Api82_FunctionContext *getConfig = [Api82 help_getConfig];
    
    if (data)
        *data = getConfig.payload;
    
    return ^MTDatacenterAddressListData *(NSData *data) {
        id response = getConfig.responseParser(data);
        if ([response isKindOfClass:[Api82_Config class]])
        {
            NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
            
            for (Api82_DcOption *dcOption in ((Api82_Config *)response).dcOptions)
            {
                NSMutableArray *array = dict[@([dcOption.pid intValue])];
                if (array == nil) {
                    array = [[NSMutableArray alloc] init];
                    dict[@([dcOption.pid intValue])] = array;
                }
                
                MTDatacenterAddress *address = [[MTDatacenterAddress alloc] initWithIp:dcOption.ipAddress port:(uint16_t)[dcOption.port intValue] preferForMedia:[dcOption.flags intValue] & (1 << 1) restrictToTcp:[dcOption.flags intValue] & (1 << 2) cdn:[dcOption.flags intValue] & (1 << 3) preferForProxy:[dcOption.flags intValue] & (1 << 4) secret:dcOption.secret];
                [array addObject:address];
            }
            return [[MTDatacenterAddressListData alloc] initWithAddressList:dict];
        }
        return nil;
    };
}

@end
