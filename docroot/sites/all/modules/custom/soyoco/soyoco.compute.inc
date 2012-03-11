<?php

/**
 * @file soyoco.compute.inc
 * Drupal 7 module - computed field callback functions
 */
 

/**
 * NODETYPE: work_shift
 */
 
/**
 * field_cooks_compute()
 *
 *
 */ 
function computed_field_field_cooks_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
 // @TODO sum primary_cook + count(joint_cooks)
  $primary_cook = $entity->field_primary_cook;
  $joint_cooks = $entity->field_joint_cooks;
  if (empty($primary_cook) && empty($joint_cooks)) { return; }
  
  $cooks = 0;
  if (!empty($primary_cook)) {
    $cooks += 1;
  }
  if (count($joint_cooks)) {
    $cooks += count($joint_cooks);
  }
  $entity_field[0]['value'] = $cooks;
}


/**
 * field_status_compute()
 *
 *
 */ 
function computed_field_field_status_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
 // @TODO check primary cook's setting, mark as OPEN or CLOSED
  $user_profile = profile2_load_by_user(user_load($entity->uid)); 
  $joint_work_shifts_welcome = $user_profile['main']->joint_work_shifts_welcome['und'][0]['value'];
  
  $status = 'OPEN';
  if (!$joint_work_shifts_welcome) {
    $status = 'CLOSED';
  }
  $entity_field[0]['value'] = $status;
}


/**
 * field_total_jars_cooked_compute()
 *
 *
 */
function computed_field_field_total_jars_cooked_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO sum field_w_jars_cooked + field_n_jars_cooked
  $sum = 0;
  $w_jars_cooked = _soyoco_view_node_fields($entity,'w_jars_cooked');
  $n_jars_cooked = _soyoco_view_node_fields($entity,'n_jars_cooked');
  
  if (empty($w_jars_cooked) && empty($n_jars_cooked)) {
    return;
  }
  
  $total_jars_cooked = $w_jars_cooked + $n_jars_cooked;
  $entity_field = $total_jars_cooked;
}

/**
 * NODETYPE: yogurt_share
 */

/**
 * field_weeks_left_compute()
 *
 *
 */
function computed_field_field_weeks_left_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // first, calculate the time lapse between now and the end, both dates rounded
  $this_week    = _soyoco_round_up_week(time());
  $share_expire = $entity->field_begin_date['und'][0]['value2'];
  $expire_week  = _soyoco_round_up_week($share_expire);
  $time_lapse = $expire_week - $this_week;
  
  // finally, store the integer value of the number of weeks in the time lapse 
  $num_days = intval(floor($time_lapse/(60*60*24)));
  $weeks_elapsed = ($num_days + 7) / 7;
  $entity_field[0]['value'] = $weeks_elapsed;
}


/**
 * field_adjusted_jar_count_compute()
 *
 *
 */
function computed_field_field_adjusted_jar_count_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // add up whole and non fat jars
  $w_jars = _soyoco_view_node_fields($entity,'w_jars');
  $n_jars = _soyoco_view_node_fields($entity,'n_jars');
  $total_jars = $w_jars + $n_jars;
  
  // get the discount from this user's main profile
  $user_profile = profile2_load_by_user(user_load($entity->uid)); 
  $discount = $user_profile['main']->field_per_share_discount['und'][0]['value'];
  
  $entity_field[0]['value'] = floatval($total_jars - $discount);
}


/**
 * field_work_share_goal_compute()
 *
 *
 */
function computed_field_field_work_share_goal_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  $term = variable_get('weeks_in_one_term', 24);
  
  // query all active shares
  $active_shares = _soyoco_query_active_shares();
  
  if (isset($active_shares['node'])) {
    $nids = array_keys($active_shares['node']);
    $nodes = entity_load('node', $nids);
  }
  
  $adjusted_jar_count_sum = 0;
  foreach ($nodes as $node) {
    if (!empty($node->field_adjusted_jar_count)) {
      dpm($node->field_adjusted_jar_count);
      $adjusted_jar_count_sum += $node->field_adjusted_jar_count['und'][0]['value'];
    }
  }
  drupal_set_message($term . ' divided by ' . $adjusted_jar_count_sum);
  $entity_field[0]['value'] = floatval($term / $adjusted_jar_count_sum);
}


/**
 * field_work_share_completed_compute()
 *
 *
 */
function computed_field_field_work_share_completed_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO Sum points earned from all past work shifts
  
  $work_share_completed = 0;
  // query all work shifts with this user as primary cook
  // foreach one, add up points (points = 1/$shift->cooks)
  // query all work shifts with this user as joint cook
  // foreach one, add up points
  $entity_field[0]['value'] = $work_share_completed;
}


/**
 * field_work_share_difference_compute()
 *
 *
 */
function computed_field_field_work_share_difference_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO Work Share Goal - Work Share Completed
  $goal       = _soyoco_view_node_fields($entity,'work_share_goal');
  $completed  = _soyoco_view_node_fields($entity,'work_share_completed');
  $entity_field[0]['value'] = floatval($goal - $completed);
}


/**
 * field_completed_percentage_compute()
 *
 *
 */
function computed_field_field_completed_percentage_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO Work Share Goal / Work Share Completed
  $goal       = _soyoco_view_node_fields($entity,'work_share_goal');
  $completed  = _soyoco_view_node_fields($entity,'work_share_completed');
  $entity_field[0]['value'] = intval(floatval(100 * $completed / $goal));
}



