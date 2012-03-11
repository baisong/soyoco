<?php

/**
 * @file soyoco.query.inc
 * Drupal 7 module - query declarations
 */


/**
 * _soyoco_query_active_shares()
 *
 *
 */  
function _soyoco_query_active_shares() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'yogurt_share')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_begin_date', 'value2', time(), '>')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  return $query->execute();
}


/**
 * _soyoco_query_user_active_shares()
 *
 *
 */ 
function _soyoco_query_user_active_shares($uid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'yogurt_share')
    ->propertyCondition('status', 1)
    ->propertyCondition('uid', $uid)
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  return $query->execute();
}


/**
 * _soyoco_query_work_shifts_as_primary()
 *
 *
 */ 
function _soyoco_query_work_shifts_as_primary($uid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'work_shift')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_primary_cook', 'uid', $uid, '=')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  return $query->execute();
}


/**
 * _soyoco_query_work_shifts_as_joint()
 *
 *
 */ 
function _soyoco_query_work_shifts_as_joint($uid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'work_shift')
    ->propertyCondition('status', 1)
    ->fieldDeltaCondition('field_joint_cooks', 'uid', $uid, '=')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  return $query->execute();
}


/**
 * _soyoco_query_final_work_shift_date()
 *
 *
 */
function _soyoco_query_final_work_shift_date() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'yogurt_share')
    ->propertyCondition('status', 1)
    ->fieldOrderBy('field_begin_date','value2','desc')
    ->range(0,1)
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  return $query->execute();

}
