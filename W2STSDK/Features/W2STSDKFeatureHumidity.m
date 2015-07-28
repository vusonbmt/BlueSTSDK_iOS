//
//  W2STSDKFeatureHumidity.m
//  W2STApp
//
//  Created by Giovanni Visentini on 10/04/15.
//  Copyright (c) 2015 STMicroelectronics. All rights reserved.
//

//
//  W2STSDKFeatureAcceleration.m
//  W2STApp
//
//  Created by Giovanni Visentini on 02/04/15.
//  Copyright (c) 2015 STMicroelectronics. All rights reserved.
//
#include <math.h>
#import "W2STSDKFeature_prv.h"
#import "W2STSDKFeatureHumidity.h"
#import "W2STSDKFeatureField.h"

#import "../Util/NSData+NumberConversion.h"

#define FEATURE_NAME @"Humidity"
#define FEATURE_UNIT @"%"
#define FEATURE_MIN 0
#define FEATURE_MAX 100
#define FEATURE_TYPE W2STSDKFeatureFieldTypeFloat

static NSArray *sFieldDesc;

@implementation W2STSDKFeatureHumidity

+(void)initialize{
    if(self == [W2STSDKFeatureHumidity class]){
        sFieldDesc = [[NSArray alloc] initWithObjects:
                      [W2STSDKFeatureField  createWithName: FEATURE_NAME
                                                      unit:FEATURE_UNIT
                                                      type:FEATURE_TYPE
                                                       min:@FEATURE_MIN
                                                       max:@FEATURE_MAX ],
                      nil];
    }
    
}


+(float)getHumidity:(W2STSDKFeatureSample *)sample{
    if(sample.data.count==0)
        return NAN;
    return[[sample.data objectAtIndex:0] floatValue];
}


-(id) initWhitNode:(W2STSDKNode *)node{
    self = [super initWhitNode:node name:FEATURE_NAME];
    return self;
}

-(NSArray*) getFieldsDesc{
    return sFieldDesc;
}


-(uint32_t) update:(uint32_t)timestamp data:(NSData*)rawData dataOffset:(uint32_t)offset{
    
    
    int16_t hum= [rawData extractLeInt16FromOffset:offset];
    
    NSArray *data = [NSArray arrayWithObject:[NSNumber numberWithFloat:hum/10.0f]];
    W2STSDKFeatureSample *sample = [W2STSDKFeatureSample sampleWithTimestamp:timestamp data:data ];
    self.lastSample = sample;
    [self notifyUpdateWithSample:sample];
    [self logFeatureUpdate:[rawData subdataWithRange:NSMakeRange(offset, 2)]
                    sample:sample];
    
    return 2;
}

@end


#import "../W2STSDKFeature+fake.h"

@implementation W2STSDKFeatureHumidity (fake)

-(NSData*) generateFakeData{
    NSMutableData *data = [NSMutableData dataWithCapacity:2];
    
    int16_t temp = FEATURE_MIN*10 + rand()%((FEATURE_MAX-FEATURE_MIN)*10);
    [data appendBytes:&temp length:2];
    
    return data;
}

@end
