<?php
/**
 * @file soyoco.util.inc
 * Drupal 7 module private functions & utilities
 */


/**
 * _soyoco_render_my_active_share()
 *
 *
 */
function _soyoco_render_user_page($uid) {
  if ($uid == 1 && arg(1) != 'override') {
    drupal_goto('admin/dashboard');
  }

  // display shares
  $shares = _soyoco_query_user_active_shares($uid);  
  if (!$shares) {
    return 'You do not currently have an active share.';
  } else {
    $output = '';
    foreach ($shares as $share) {
      $output .= theme('soyoco_yogurt_share', array('node' => $share));
        // attach work shifts remaining
      if (!isset($share->field_end_date['und'][0]['value'])) {
        return $output;
      }
      $shifts = _soyoco_query_work_shifts_remaining($share->field_end_date['und'][0]['value']);
      if ($shifts) {
        foreach ($shifts as $shift) {
          // dpm($shift->field_primary_cook);
          $output .= theme('soyoco_work_shift',array('node' => $shift));
        }
      } else {
        dpm($share);
        $output .= '<p>No remaining work shifts!</p>';
      }
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
 * @param int    $end_date
 * @param string $day
 * @return
 */
function _soyoco_round_up_week($end_time, $day = 'Thursday') {
  $final_time = strtotime('next ' . $day);
  $ONE_WEEK = 60*60*24*7;
  
  while ($final_time < $end_time) {
    $final_time += $ONE_WEEK;
  }
  return $final_time;
}


/**
 * soyoco_view_node_fields()
 *
 *
 */
function _soyoco_view_node_fields($node, $fieldname, $entity_type='node') {
  $output = '';
  $fname = 'field_' . $fieldname;
  $fields = field_get_items($entity_type, $node, $fname);
  if (isset($node->$fname) && empty($node->$fname)) {
    return;
  }
  if (is_array($fields)) {
    foreach ($fields as $key=>$value) {
      $this_field = field_view_value($entity_type, $node, $fname, $fields[$key]);
      $output .= render($this_field); 
    }
  } else {
    drupal_set_message('No field_' . $fieldname . ' on node ' . $node-> type . $node->nid);
  }
  return $output;
}


/**
 * _soyoco_add_missing_work_shifts()
 */
function _soyoco_add_missing_work_shifts($start_date,$end_date) {
  //dpm($start_date,__FUNCTION__);dpm($end_date,__FUNCTION__);
  $plus_one_week = ' +1 week';
  $iter_date = $start_date;
  while ($iter_date < $end_date) {
    $nodes = _soyoco_query_shift_by_date($iter_date);
    // @FIXME should make sure it's a thursday!
    if (empty($nodes)) { 
      _soyoco_add_work_shift($iter_date);
    }
    $iter_date = strtotime(date("Y-m-d",$iter_date) . ' +1 week');
  }
}


/**
 * _soyoco_update_shares()
 *
 *
 */
function _soyoco_update_shares() {
  $nodes = _soyoco_query_all_shares();
  if (empty($nodes)) {
    drupal_set_message('Error! No shares?');
    return;
  }
  foreach ($nodes as $node) {
    //dpm($node);
    node_save($node);
  }
}


/**
 * _soyoco_update_shares()
 *
 *
 */
function _soyoco_update_shifts() {
  $nodes = _soyoco_query_all_shifts();
  if (empty($nodes)) {
    drupal_set_message('Error! No shifts?');
    return;
  }
  foreach ($nodes as $node) {
    node_save($node);
  }
}


/**
 *
 *
 */
function _soyoco_work_shift_title($datetime) {
  // return something like "January C" for the third week of january
  $week_names = array('A','B','C','D','E');
  $week_ordinal = 0;
  
  $month = date('F',$datetime); // i.e. "January"
  $i = strtotime(date("Y-m-d",$datetime) . ' -1 week');
  $i_month = date('F',$i);
  while ($i_month == $month) {
    // keep subtracting a week
    $i = strtotime(date("Y-m-d",$i) . ' -1 week');
    $i_month = date('F',$i);
    $week_ordinal += 1;
  }
  $title = $month . ' ' . $week_names[$week_ordinal];
  return $title;
}


/**
 *
 *
 */
function _soyoco_add_work_shift($datetime) {
  $date =  array('date' => date("Y-m-d",$datetime), 'time' => '00:00:00');
  $name = 'soyoco';
  $title = _soyoco_work_shift_title($datetime);
  $values = array(
    'yogurt_week_date' => $date,
  );
  //drupal_set_message('adding workshift ' . $title . ' for ' . $date);
  _soyoco_add_node('work_shift',$title,$name,$values);
}


/**
 *
 *
 */
function _soyoco_add_node($type,$title = 'untitled',$name = 'soyoco',$values = array()) {
  module_load_include('inc', 'node', 'node.pages');
  $node = (object) array(
    'type' => $type,
    'language' => LANGUAGE_NONE
  );
  node_object_prepare($node);
     
  $form_state = array();  
  $form_state['values']['title'] = $title;  
  $form_state['values']['name'] = $name;  
  $form_state['values']['op'] = t('Save');
  if (count($values)) {
    foreach ($values as $fieldname=>$value) {
      //dpm($fieldname,__FUNCTION__);dpm($value,__FUNCTION__);
      $form_state['values']['field_' . $fieldname][$node->language][0]['value'] = $value;  
    }
  }
  drupal_form_submit("{$node->type}_node_form", $form_state, $node);
}




