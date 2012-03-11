<?php

/**
 * @file soyoco.util.inc
 * Drupal 7 module private functions & utilities
 */

/**
 * _soyoco_display_user_info()
 *
 *
 */
function _soyoco_render_my_active_share() {
  global $user;
  $query = db_select('node', 'n')
    ->fields('n')
    ->condition('uid',$user->uid,'=')
    ->orderBy('created', 'DESC');
  
  $result = $query->execute();
  $count = $result->rowCount();
  
  $output = '';
  if ($count < 1) {
    $empty_text = 'You do not currently have an active share.';
    $output .= $empty_text;
  } else {
    while($record = $result->fetchAssoc()) {
      $output .= theme('soyoco_yogurt_share',array('node' => node_load($record['nid'])));
    }
  }
  return $output;
}


/**
 * soyoco_weeks_til_expiry($end_date)
 *
 * @param  $end_date      - the final date of a work shift
 * @return $weeks_elapsed - the number of weeks between now and then
 *
 * note: this function "rounds up" from now and end date to soonest Saturday.
 */
function soyoco_weeks_til_expiry($end_date) {
  $this_week    = _soyoco_round_up_week(time());
  $expire_week  = _soyoco_round_up_week($end_date);
  $interval = $expire_week - $this_week;
  $num_days = intval(floor($interval/(60*60*24)));
  $weeks_elapsed = ($num_days + 7) / 7;
  return $weeks_elapsed;
}


/**
 * soyoco_next_shareweek_ends()
 *
 * @param  $end_date
 * @param $day
 * @return
 */
function _soyoco_round_up_week($end_date,$day = 'Saturday') {
  $final_date = strtotime('next ' . $day);
  $ONE_WEEK = 60*60*24*7;
  
  while ($final_date < strtotime($end_date)) {
    $final_date += $ONE_WEEK;
  }
  return $final_date;
}


/**
 * soyoco_view_node_fields()
 *
 *
 */
function _soyoco_view_node_fields($node,$fieldname,$entity_type='node') {
  $output = '';
  $fname = 'field_' . $fieldname;
  $fields = field_get_items($entity_type, $node, $fname);

  foreach ($fields as $key=>$value) {
    $this_field = field_view_value($entity_type, $node, $fname, $fields[$key]);
    $output .= render($this_field); 
  } 
  return $output;
}
