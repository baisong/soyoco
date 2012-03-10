<?php
/**
 *
 *
 *
 */


/**
 *
 *
 *
 */
function soyoco_weeks_til_expiry($end_date){
  $this_week = soyoco_next_shareweek_ends(time());
  $expire_week = soyoco_next_shareweek_ends($end_date);
  $interval = $expire_week - $this_week;
  $num_days = intval(floor($interval/(60*60*24)));
  $num_weeks = ($num_days + 7) / 7;
  return $num_weeks;
}


/**
 *
 *
 *
 */
function soyoco_next_shareweek_ends($end_date,$day = 'Saturday'){
  $ret = strtotime('next ' . $day);
  $ONE_WEEK = 60*60*24*7;
  
  while ($ret < strtotime($end_date)) {
    $ret += $ONE_WEEK;
  }
  return $ret;
}

/**
 *
 *
 *
 */
function soyoco_theme_share($wjar,$njar,$weeks_left,$duration){
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
 *
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
