//
//  W2STSDKFeatureHumidity.h
//  W2STApp
//
//  Created by Giovanni Visentini on 10/04/15.
//  Copyright (c) 2015 STMicroelectronics. All rights reserved.
//

#import "W2STSDKFeature.h"

/**
 *  feature that contains the data from an humidity sensor
 */
@interface W2STSDKFeatureHumidity : W2STSDKFeature

/**
 *  % of humidity extracted by the sensor
 *
 *  @param data sample read from the node
 *
 *  @return % of humidity extracted by the sensor
 */
+(float)getHumidity:(W2STSDKFeatureSample*)data;

@end
