/*******************************************************************************
 * COPYRIGHT(c) 2015 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef BlueSTSDK_BlueSTSDKNode_h
#define BlueSTSDK_BlueSTSDKNode_h

#import <Foundation/Foundation.h>
#import "BlueSTSDKManager.h"

@class BlueSTSDKDebug;
@class BlueSTSDKConfigControl;
@class BlueSTSDKFeature;


@protocol BlueSTSDKNodeBleConnectionParamDelegate;
@protocol BlueSTSDKNodeStateDelegate;

/**
 * Class that represent a remote device that will export some data (as {@link BlueSTSDKFeature})
 * that the user can query or ask to be notify when they change.
 *
 *
 * @note
 * In case that the node require the a secure connection with the pin it can happen
 * that the notification subscription are sent before the connection completed.
 * It is not possible have callback/notification when the connection completed.
 * For this reason we try to subscribe the feature every second until we didn't
 * receive some data from it.
 *
 * @author STMicroelectronics - Central Labs.
 */
NS_CLASS_AVAILABLE(10_7, 5_0)
@interface BlueSTSDKNode : NSObject 

/**
 *  possible node mode
 */
typedef NS_ENUM(NSInteger, BlueSTSDKNodeMode){
    /**
     *  enter in the update mode through the usb port
     */
    BlueSTSDKNodeModeUSB_DFU,
    /**
     *  enter in the update mode through the ble connection
     */
    BlueSTSDKNodeModeOTA_BLE_DFU,
    /**
     *  normal state
     */
    BlueSTSDKNodeModeApplication
};

/**
 *  possible connection state
 */
typedef NS_ENUM(NSInteger, BlueSTSDKNodeState){
    /**
     *  start state
     */
    BlueSTSDKNodeStateInit          = 0,
    /**
     *  node available, waiting for a connection command
     */
    BlueSTSDKNodeStateIdle          = 1,
    /**
     *  connection in progress
     */
    BlueSTSDKNodeStateConnecting    = 2,
    /**
     *  node connected
     */
    BlueSTSDKNodeStateConnected     = 3,
    /**
     *  disconnection in progress
     */
    BlueSTSDKNodeStateDisconnecting = 4,
    /**
     *  error during a connection
     */
    BlueSTSDKNodeStateLost          = 5,
    /**
     *  the node does't respond to a connection
     */
    BlueSTSDKNodeStateUnreachable   = 6,
    /**
     *  end node
     */
    BlueSTSDKNodeStateDead          = 7,
};

/**
 *  board type
 */
typedef NS_ENUM(NSInteger, BlueSTSDKNodeType){
    /**
     *  unknown board
     */
    BlueSTSDKNodeTypeGeneric      = 0x00,
    /**
     *  STEVAL WESU1 board
     */
    BlueSTSDKNodeTypeSTEVAL_WESU1         = 0x01,
    /**
     *  nucleo + ble expansion board
     */
    BlueSTSDKNodeTypeNucleo       = 0x80,
};

/**
 *  current board status
 */
@property (assign, nonatomic,readonly) BlueSTSDKNodeState state;

/**
 *  board type
 */
@property (assign, nonatomic,readonly) BlueSTSDKNodeType type;

/**
 *  node name, is not guaranteed that this string is unique
 */
@property (retain, readonly) NSString *name;

/**
 *  node address, if available in the advertise otherwise nil
 */
@property (retain, readonly) NSString *address;

/**
 *  version of the protocol
 */
@property (readonly) unsigned char protocolVersion;

/**
 *  unique string that identify the node
 */
@property (retain, readonly) NSString *tag;

/**
 *  date of the last rssi update package
 */
@property (readonly) NSDate *rssiLastUpdate;

/**
 *  last rssi signal value
 */
@property (retain, readonly) NSNumber *RSSI;

/**
 *  object that we can use for access to the debug console or nil if it is absent
 */
@property (retain, readonly) BlueSTSDKDebug *debugConsole;

/**
 *  object that we can use for access to the config node or nil if it is absent
 */
@property (readonly) BlueSTSDKConfigControl *configControl;

/**
 *  tx power used from the board
 */
@property (retain, readonly) NSNumber *txPower;

/**
 *  convert the enum value to a string
 *
 *  @param state enum with the current board status
 *
 *  @return a string representation of the enum value
 */
+(NSString*) stateToString:(BlueSTSDKNodeState)state;

/**
 *  add a delegate where notify the change in the connection parameters
 *
 *  @param delegate object where notify change of rssi or transmission power
 */
-(void) addBleConnectionParamiterDelegate:(id<BlueSTSDKNodeBleConnectionParamDelegate>)delegate;

/**
 *  remove a delegate of type {@link BlueSTSDKNodeBleConnectionParamDelegate}
 *
 *  @param delegate delegate to remove
 */
-(void) removeBleConnectionParamiterDelegate:(id<BlueSTSDKNodeBleConnectionParamDelegate>)delegate;

/**
 *  add a delegate where notify change of the node connection status
 *
 *  @param delegate delegate to add
 */
-(void) addNodeStatusDelegate:(id<BlueSTSDKNodeStateDelegate>)delegate;

/**
 *  remove a delegate of type {@link BlueSTSDKNodeStateDelegate}
 *
 *  @param delegate delegate to remove
 */
-(void) removeNodeStatusDelegate:(id<BlueSTSDKNodeStateDelegate>)delegate;

/**
 *  compare two nodes, the node are equal if the tag value is the same
 *
 *  @param node node to compare
 *
 *  @return true if node.tag == self.tag
 */
-(BOOL) equals:(BlueSTSDKNode *)node;

/**
 *  get the available features, there is a feature for each bit set in the advertise,
 *  if the linked characteristics is not find in the node the feature is not enabled
 *
 *  @return array of {@link BlueSTSDKFeature} that the node can export
 */
-(NSArray*) getFeatures;

/**
 *  return a particular feature or null if it is not available
 *
 *  @param type type of feature that we want have
 *
 *  @return null if the feature doesn't exit, otherwise the class that impement 
 * the request feature
 */
-(BlueSTSDKFeature*)getFeatureOfType:(Class)type;

/**
 *  request to update the rssi value
 *  the answer is notify through the {@link BlueSTSDKNodeBleConnectionParamDelegate::node:didChangeRssi:}
 * callback
 */
-(void) readRssi;

/**
 *  open a connection with this node
 * \par
 * when the connection is completed the the class notify it with a callback
 * on {@link BlueSTSDKNodeStateDelegate::node:didChangeState:prevState:}
 */
-(void) connect;

/**
 *  tell if this node is connected
 *
 *  @return true if the node is in the state BlueSTSDKNodeStateConnected
 */
-(BOOL) isConnected;

/**
 *  close the connection with this node
 */
-(void) disconnect;

/**
 *  request to read the value of a particular feature, the feature will manage to notify you the new value
 *
 *  @param feature feature to read
 *
 *  @return true if the request is send correctly
 */
-(BOOL) readFeature:(BlueSTSDKFeature*)feature;

/**
 *  tell if the notifications for a feature are enabled
 *
 *  @param feature feature
 *
 *  @return true if you enable the notification for this feature
 */
-(BOOL) isEnableNotification:(BlueSTSDKFeature*)feature;

/**
 *  enable the notification for a particular feature, the feature will manage 
 * to notify you the new values
 *
 *  @param feature feature to notify
 *
 *  @return true if the request is send correctly
 */
-(BOOL) enableNotification:(BlueSTSDKFeature*)feature;

/**
 *  disable the notification for a particular feature
 *
 *  @param feature feature to stop notify
 *
 *  @return true if the request is send correctly
 */
-(BOOL) disableNotification:(BlueSTSDKFeature*)feature;

@end

/**
 *  protocol used by the node for notify a connection state change
 */
@protocol BlueSTSDKNodeStateDelegate <NSObject>

@required
/**
 *  notify to the user that the node change its status
 *
 *  @param node      node that call the delegate, node that change the status
 *  @param newState  new node status
 *  @param prevState old node status
 */
- (void) node:(BlueSTSDKNode *)node didChangeState:(BlueSTSDKNodeState)newState prevState:(BlueSTSDKNodeState)prevState;
@end

/**
 *  protocol used by the node for notify a change in the BLE connection parameters
 */
@protocol BlueSTSDKNodeBleConnectionParamDelegate <NSObject>

@required
/**
 *  notify to the user a new rssi value
 *
 *  @param node    node that has a new rssi value
 *  @param newRssi new rssi value for the nodes
 */
- (void) node:(BlueSTSDKNode *)node didChangeRssi:(NSInteger)newRssi;

@optional
/**
 *  notify to the user that the tx power changed
 *
 *  @param node     node that change the tx power
 *  @param newPower new tx power for the node
 *  @note implement this method is optional
 */
- (void) node:(BlueSTSDKNode *)node didChangeTxPower:(NSInteger)newPower;

@end

#endif

