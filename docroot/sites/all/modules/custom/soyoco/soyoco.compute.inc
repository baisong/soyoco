<?php

/**
 *
 *
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
}


/**
 * field_status_compute()
 *
 *
 */ 
function computed_field_field_status_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
 // @TODO check primary cook's setting, mark as OPEN or CLOSED
}


/**
 * field_total_jars_cooked_compute()
 *
 *
 */
function computed_field_field_total_jars_cooked_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO sum field_w_jars_cooked + field_n_jars_cooked
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
  // @TODO Term/Sum of all Per Member Adjusted Jar Counts
}


/**
 * field_work_share_completed_compute()
 *
 *
 */
function computed_field_field_work_share_completed_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO Sum points earned from all past work shifts
}


/**
 * field_work_share_difference_compute()
 *
 *
 */
function computed_field_field_work_share_difference_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO Work Share Goal - Work Share Completed
}


/**
 * field_completed_percentage_compute()
 *
 *
 */
function computed_field_field_completed_percentage_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // @TODO Work Share Goal / Work Share Completed
}



