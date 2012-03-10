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
    $output .= 'You do not currently have an active share.';
  } else {
    while($record = $result->fetchAssoc()) {
      $node = node_load($record['nid']);
      $wjars_val = soyoco_view_node_fields($node,'w_jars');
      $njars_val = soyoco_view_node_fields($node,'n_jars');
      $duration = soyoco_view_node_fields($node,'begin_date');
      $end_date = $node->field_begin_date['und'][0]['value2'];
      $weeks_left = soyoco_weeks_til_expiry($end_date);
      $output .= soyoco_theme_share($wjars_val,$njars_val,$weeks_left,$duration);
    }
  }
  return $output;
}


/**
 * soyoco_weeks_til_expiry($end_date)
 *
 *
 */
function soyoco_weeks_til_expiry($end_date) {
  $this_week = soyoco_next_shareweek_ends(time());
  $expire_week = soyoco_next_shareweek_ends($end_date);
  $interval = $expire_week - $this_week;
  $num_days = intval(floor($interval/(60*60*24)));
  $num_weeks = ($num_days + 7) / 7;
  return $num_weeks;
}


/**
 * soyoco_next_shareweek_ends()
 *
 *
 */
function soyoco_next_shareweek_ends($end_date,$day = 'Saturday') {
  $ret = strtotime('next ' . $day);
  $ONE_WEEK = 60*60*24*7;
  
  while ($ret < strtotime($end_date)) {
    $ret += $ONE_WEEK;
  }
  return $ret;
}

/**
 * soyoco_theme_share($wjar,$njar,$weeks_left,$duration)
 *
 *
 */
function soyoco_theme_share($wjar,$njar,$weeks_left,$duration) {
  $output  = '<div class="row"><div class="span5 well" style="height:100px"><div class="row"><div class="span3">';
  for ($i=0;$i<$wjar;$i++) {
    $output .= '[W]';
  }
  for ($i=0;$i<$njar;$i++) {
    $output .= '[N]';
  }
  $output .= '</div><div class="span2"><p><strong>Whole Fats:</strong> ' . strval($wjar) . '</p>';
  $output .= '<p><strong>Non Fat:</strong> ' . strval($njar) . '</p></div></div></div>';
  $output .= '<div class="span3 well" style="height:100px"><div class="jumbotron"><h1>' . strval($weeks_left) . ' <small>weeks left</small></h1></div>';  
  $output .= '<div style="color:#333"><dl><dt>Share Duration:</dt><dd>' . strval($duration) . '</dd></dl></div></div>';  
  return $output;
}


/**
 * soyoco_view_node_fields()
 *
 *
 */
function soyoco_view_node_fields($node,$fieldname) {
  $output = '';
  $fname = 'field_' . $fieldname;
  $fields = field_get_items('node', $node, $fname);

  foreach ($fields as $key=>$value) {
    $this_field = field_view_value('node', $node, $fname, $fields[$key]);
    $output .= render($this_field); 
  } 
  return $output;
}
