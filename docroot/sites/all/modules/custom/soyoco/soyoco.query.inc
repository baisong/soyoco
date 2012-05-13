<?php

/**
 * @file soyoco.query.inc
 * Drupal 7 module - query declarations
 */

/**
 * _soyoco_query_user_active_shares()
 *
 * @param $result return value of an EntityFieldQuery where entity_type=node
 */ 
function _soyoco_query_load_nodes($query_result) {
  if (isset($query_result['node'])) {
    $nids = array_keys($query_result['node']);
    $nodes = entity_load('node', $nids);
    return $nodes;
  }
  return FALSE;
}

/**
 * _soyoco_query_active_shares()
 *
 * @FIXME this should only do ACTIVE shares
 */  
function _soyoco_query_all_shares() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'yogurt_share')
    ->propertyCondition('status', 1)
    //->fieldCondition('field_start_date', 'value', time(), '>')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  $result = $query->execute();
  return _soyoco_query_load_nodes($result);
}

/**
 * _soyoco_query_active_shares()
 *
 *
 */  
function _soyoco_query_all_shifts() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'work_shift')
    ->propertyCondition('status', 1)
    //->fieldCondition('field_start_date', 'value', time(), '>')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  $result = $query->execute();
  return _soyoco_query_load_nodes($result);
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
  
  $result = $query->execute();
  return _soyoco_query_load_nodes($result);
}


/**
 * _soyoco_query_work_shifts_as_primary()
 *
 * @TODO: enforce week_date <= $enddate fieldCondition.
 * @param $startdate (timestamp) 
 */
function _soyoco_query_work_shifts_as_primary($uid, $startdate, $enddate) {
  $date1 = date("Y-m-d", $startdate) . ' 00:00:00';
  $date2 = date("Y-m-d", $enddate) . ' 00:00:00';
  //dpm($uid);
  //dpm($startdate);
  //dpm($enddate);
  
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'work_shift')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_primary_cook', 'target_id', $uid, '=')
    ->fieldCondition('field_yogurt_week_date', 'value', $startdate, '>=')
    //->fieldCondition('field_yogurt_week_date', 'value', $enddate, '<=')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  $result = $query->execute();
  return _soyoco_query_load_nodes($result);
}


/**
 * Return all work shifts where this user is joing, in active share ONLY
 *
 */ 
function _soyoco_query_work_shifts_as_joint($uid, $startdate, $enddate) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'work_shift')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_joint_cooks', 'target_id', $uid, '=')
    ->fieldCondition('field_yogurt_week_date', 'value', $startdate, '>=')
    ->fieldCondition('field_yogurt_week_date', 'value', $enddate, '<=')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  $result = $query->execute();
  return _soyoco_query_load_nodes($result);
}


/**
 *
 */
function _soyoco_query_shift_by_date($datetime) {
  $date = date("Y-m-d",$datetime) . ' 00:00:00';
  
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'work_shift')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_yogurt_week_date','value', $date, '=')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  $result = $query->execute();
  return _soyoco_query_load_nodes($result);  
}


/**
 * _soyoco_query_work_shifts_remaining()
 *
 * @TODO: make this only look for active share work shifts
 */ 
function _soyoco_query_work_shifts_remaining($datetime) {
  $now = strtotime(date("Y-m-d",time()) . '-1 Thursday');
  $now_date = date("Y-m-d",$now) . ' 00:00:00';
  $end_date = date("Y-m-d",$datetime) . ' 00:00:00';
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'work_shift')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_yogurt_week_date','value', $end_date, '<=')
    ->fieldCondition('field_yogurt_week_date','value', $now_date, '>=')
    ->fieldOrderBy('field_yogurt_week_date','value','asc')
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  $result = $query->execute();
  return _soyoco_query_load_nodes($result);
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
    ->fieldOrderBy('field_end_date', 'value', 'desc')
    ->range(0,1)
    ->addMetaData('account', user_load(1)); // run the query as user 1  
  
  $result = $query->execute();
  $nodes = _soyoco_query_load_nodes($result);
  foreach ($nodes as $node) {
    return $node->field_end_date['und'][0]['value'];  
  }
  return FALSE;
}


