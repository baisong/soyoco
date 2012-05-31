<?php

/**
 * @file soyoco.compute.inc
 * Drupal 7 module - computed field callback functions
 * 
 * Variable Nomenclature: date suffixes
 *
 *  _time     = int     1338488440
 *  _datestr  = string  '2011-10-27'
 *  _datetime = string  '2011-10-27 00:00:00'
 *  _fmt      = string  '5/1'
 */
 
 
/**
 *
 *
 */
function computed_field_field_is_active_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // Shares are inactive unless the current time is within the share period.
  if (isset($entity->type) && $entity->type === 'yogurt_share') {
    // must be after start date
    $now_time = time();
    $start_date_datetime = ($entity->field_start_date['und'][0]['value']);
    $start_date_time = strtotime($start_date_datetime);
    $share_has_begun = ((int)$now_time > (int)$start_date_time);
    
    // and must be before end date
    $end_date_time = $entity->field_end_date['und'][0]['value'];
    $share_not_ended = ((int)$now_time < (int)$end_date_time);
    
    if ($share_has_begun && $share_not_ended) { $entity_field[0]['value'] = 1; return; }
    $entity_field[0]['value'] = 0; return;
  }
  
  // Shifts are inactive unless at least one cook has an active share in range, or it's in the future.
  else if (isset($entity->type) && $entity->type === 'work_shift') {
    // if work date in the future, it's active.
    $now_time = time();
    $yogurt_week_date_datetime = $entity->field_yogurt_week_date['und'][0]['value'];
    $yogurt_week_date_time = strtotime($yogurt_week_date_datetime);
    $shift_is_in_future = ((int)$now_time < (int)$yogurt_week_date_time);
    if ($shift_is_in_future) { $entity_field[0]['value'] = 1; return; }

    // if it's old, but there are cooks,
    $shift_has_cooks = sizeof($entity->field_joint_cooks) > 0;
    
    if ($shift_has_cooks) {
      $cooks = $entity->field_joint_cooks['und'];
      // ...and a cook's share is active and started before date, it's active.
      foreach ($cooks as $cook_array) {
        if (!isset($cook_array['target_id'])) continue;
        $cook_active_shares_array = _soyoco_query_user_active_shares($cook_array['target_id']);
        if (!is_array($cook_active_shares_array)) continue;
        $cook_active_share = array_shift($cook_active_shares_array);
        $cook_active_share_start_date_datetime = $cook_active_share->field_start_date['und'][0]['value'];
        $cook_active_share_start_date_time = strtotime($cook_active_share_start_date_datetime);
        $cook_active_share_had_started = ((int)$cook_active_share_start_date_time < (int)$yogurt_week_date_time);
        if ($cook_active_share_had_started) { $entity_field[0]['value'] = 1; return; }
      }
    }
    $entity_field[0]['value'] = 0; return;
  }
}


/**
 * NODETYPE: work_shift
 */

 
/**
 * Counts the number of signed up cooks on a work shift.
 */ 
function computed_field_field_cooks_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  $cooks = $entity->field_joint_cooks;
  if (empty($cooks)) { return; }
    
  $cooks_count = 0;
  if (sizeof($cooks['und']) > 0) { $cooks_count = sizeof($cooks['und']); }
  $entity_field[0]['value'] = $cooks_count;
}


/**
 * field_status_compute()
 *
 *
 *
function computed_field_field_status_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  //dpm($entity);
  // if this date has passed, mark as "DONE"
  $time = strtotime($entity->field_yogurt_week_date['und'][0]['value']);
  $now = strtotime(date("Y-m-d",time()) . '-1 day');
  $is_done = ((int)$time <= (int)$now);
  if ($is_done) {
    $entity_field[0]['value'] = 'DONE';
    return;
  }
  // if there is no primary cook, mark as "OPEN"
  $primary_cook = $entity->field_primary_cook;
  if (!isset($primary_cook['und'][0]['target_id'])) {
    $entity_field[0]['value'] = 'OPEN';
    return;
  }
  // if the cook does not welcome joint shifts, mark as "CLOSED"
  $user_profile = profile2_load_by_user(user_load($primary_cook['und'][0]['target_id'])); 
  $joint_welcome_field = $user_profile['main']->field_joint_work_shifts_welcome;
  $joint_welcome = (!empty($joint_welcome_field['und'][0]['value']));
  if (!$joint_welcome) {
    $status = 'CLOSED';
  }
  // default to "OPEN"
  $entity_field[0]['value'] = 'OPEN';
} */


/**
 * field_total_jars_cooked_compute()
 *
 *
 */
function computed_field_field_total_jars_cooked_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // sum field_w_jars_cooked + field_n_jars_cooked
  $w_jars_cooked = _soyoco_view_node_fields($entity,'w_jars_cooked');
  $n_jars_cooked = _soyoco_view_node_fields($entity,'n_jars_cooked');
  if ($w_jars_cooked && $n_jars_cooked) {
    return;
  }
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
  if (!isset($entity->field_end_date['und'][0]['value'])) {
    return;
  }
  $this_week = _soyoco_round_up_week(time());
  //dpm($this_week);
  $end_date_time = $entity->field_end_date['und'][0]['value'];
  //dpm($end_date_time);
  $expire_week  = _soyoco_round_up_week($end_date_time);
  
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
  $w_jars = _soyoco_view_node_fields($entity, 'w_jars');
  $n_jars = _soyoco_view_node_fields($entity, 'n_jars');
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
  $all_active_shares_array = _soyoco_query_all_active_shares();
  if (!is_array($all_active_shares_array)) { return; }
  
  $adjusted_jar_count_sum = 0;
  foreach ($all_active_shares_array as $share_node) {
    if (!empty($share_node->field_adjusted_jar_count)) {
      $adjusted_jar_count_sum += $share_node->field_adjusted_jar_count['und'][0]['value'];
    }
  }
  $entity_field[0]['value'] = floatval($adjusted_jar_count * $term / $adjusted_jar_count_sum);
}

/**
 * field_work_share_goal_display()
 */
function computed_field_field_work_share_goal_display($field, $entity_field_item, $entity_lang, $langcode) {
  return number_format(round($entity_field_item['value'], 1), 1);
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
  $uid   = $entity->uid;
  $date1 = strtotime($entity->field_start_date['und'][0]['value']);
  $date2 = strtotime(date('Y-m-d', $entity->field_end_date['und'][0]['value']));
  
  // query all work shifts with this user as cook
  // @TODO verify only counts active shares.
  $work_shifts_cooked_array = _soyoco_query_work_shifts_as_joint($entity->uid, $date1, $date2);
  
  if (is_array($work_shifts_cooked_array)) {
    foreach ($work_shifts_cooked_array as $shift_node) {
      $cooks = $shift_node->field_cooks['und'][0]['value'];
      $points = floatval(1 / $cooks);
      //dpm($points);
      $work_share_completed += $points;
    }
  }
  
  $entity_field[0]['value'] = $work_share_completed;
}


/**
 * field_work_share_goal_display()
 */
function computed_field_field_work_share_completed_display($field, $entity_field_item, $entity_lang, $langcode) {
  return number_format(round($entity_field_item['value'], 1), 1);
}

/**
 * field_work_share_difference_compute()
 *
 *
 */
function computed_field_field_work_share_difference_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // Work Share Goal - Work Share Completed
  $goal       = _soyoco_view_node_fields($entity, 'work_share_goal');
  $completed  = _soyoco_view_node_fields($entity, 'work_share_completed');
  $entity_field[0]['value'] = floatval($goal - $completed);
}


/**
 * field_work_share_difference_display()
 */
function computed_field_field_work_share_difference_display($field, $entity_field_item, $entity_lang, $langcode) {
  return round($entity_field_item['value'], 1);
}


/**
 * field_completed_percentage_compute()
 *
 *
 */
function computed_field_field_completed_percentage_compute(&$entity_field, $entity_type, $entity, $field, $instance, $langcode, $items) {
  // Work Share Goal / Work Share Completed
  $goal       = _soyoco_view_node_fields($entity, 'work_share_goal');
  $completed  = _soyoco_view_node_fields($entity, 'work_share_completed');
  if ($goal <= 0) {
    $entity_field[0]['value'] = '100%';
    return;
  }
  $entity_field[0]['value'] = intval(floatval(100 * $completed / $goal)) . '%';
}



