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
 // sum primary_cook + count(joint_cooks)
  $primary_cook = $entity->field_primary_cook;
  $joint_cooks = $entity->field_joint_cooks;

  if (empty($primary_cook['und']) && empty($joint_cooks['und'])) { return; }
  
  $cooks = 0;
  if (!empty($primary_cook['und'])) {
    $cooks += 1;
  }
  if (count($joint_cooks['und'])) {
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
  $joint_work_shifts_welcome = $user_profile['main']->field_joint_work_shifts_welcome['und'][0]['value'];
  
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
  // sum field_w_jars_cooked + field_n_jars_cooked
  $sum = 0;
  $w_jars_cooked = _soyoco_view_node_fields($entity,'w_jars_cooked');
  $n_jars_cooked = _soyoco_view_node_fields($entity,'n_jars_cooked');
  $total_jars_cooked = $w_jars_cooked + $n_jars_cooked;
  $entity_field[0]['value'] = intval($total_jars_cooked);
}

/**
 * NODETYPE: yogurt_share
 */

/**
 * field_end_date_compute()
 *
 *
 */
function computed_field_field_end_date_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  $start_date = $entity->field_start_date['und'][0]['value'];
  $duration = $entity->field_duration['und'][0]['value'];
  if (!isset($start_date) || !isset($duration)) {
    return;
  }
  $offset = ' + ' . $duration . ' Thursdays';
  $end_date = strtotime(date("Y-m-d", strtotime($start_date)) . $offset);
  //dpm($start_date);dpm($end_date);
  _soyoco_add_missing_work_shifts(strtotime($start_date), $end_date);
  $entity_field[0]['value'] = $end_date;
}

/**
 * field_end_date_display()
 */
function computed_field_field_end_date_display($field, $entity_field_item, $entity_lang, $langcode) {
  $date_text = date('D, n/d',$entity_field_item['value']);
  /*
  $three_weeks_hence = strtotime(date("Y-m-d", strtotime(time())) . ' +3 years');
  if ($entity_field_item['value'] < strtotime($three_weeks_hence)) {
    return '<span class="label label-danger">' . $date_text . '</span>';
  }
  */
  return $date_text;
}

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
  
  $adjusted_jar_count = floatval($total_jars - $discount);
  if ( $adjusted_jar_count < 1) {
    $adjusted_jar_count = 1;
  }
  $entity_field[0]['value'] = $adjusted_jar_count;
}


/**
 * field_work_share_goal_compute()
 *
 *
 */
function computed_field_field_work_share_goal_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  $term = variable_get('weeks_in_one_term', 24);
  $adjusted_jar_count = $entity->field_adjusted_jar_count['und'][0]['value'];
  // query all active shares
  $nodes = _soyoco_query_active_shares();
  if (!$nodes) {
    return;
  }
  $adjusted_jar_count_sum = 0;
  foreach ($nodes as $node) {
    if (!empty($node->field_adjusted_jar_count)) {
      $adjusted_jar_count_sum += $node->field_adjusted_jar_count['und'][0]['value'];
    }
  }
  $entity_field[0]['value'] = floatval($adjusted_jar_count * $term / $adjusted_jar_count_sum);
}

/**
 * field_work_share_goal_display()
 */
function computed_field_field_work_share_goal_display($field, $entity_field_item, $entity_lang, $langcode) {
  return round($entity_field_item['value'],1);
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
  $nodes = _soyoco_query_work_shifts_as_primary($entity->uid);
  if ($nodes) {  
    // foreach one, add up points (points = 1/$shift->cooks)
    foreach ($nodes as $node) {
      $work_share_completed += floatval(1/$node->field_cooks['und'][0]['value']);
    }
  }
  // query all work shifts with this user as joint cook
  $nodes = _soyoco_query_work_shifts_as_joint($entity->uid);
  if ($nodes) {
    foreach ($nodes as $node) {
      $work_share_completed += floatval(1/$node->field_cooks['und'][0]['value']);
    }
  }
  $entity_field[0]['value'] = $work_share_completed;
}


/**
 * field_work_share_difference_compute()
 *
 *
 */
function computed_field_field_work_share_difference_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // Work Share Goal - Work Share Completed
  $goal       = _soyoco_view_node_fields($entity,'work_share_goal');
  $completed  = _soyoco_view_node_fields($entity,'work_share_completed');
  $entity_field[0]['value'] = floatval($goal - $completed);
}


/**
 * field_work_share_difference_display()
 */
function computed_field_field_work_share_difference_display($field, $entity_field_item, $entity_lang, $langcode) {
  return round($entity_field_item['value'],1);
}


/**
 * field_completed_percentage_compute()
 *
 *
 */
function computed_field_field_completed_percentage_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // Work Share Goal / Work Share Completed
  $goal       = _soyoco_view_node_fields($entity,'work_share_goal');
  $completed  = _soyoco_view_node_fields($entity,'work_share_completed');
  if ($goal <= 0) {
    $entity_field[0]['value'] = '100%';
    return;
  }
  $entity_field[0]['value'] = intval(floatval(100 * $completed / $goal)) . '%';
}



