<?php
/**
 * variables:
 * $node 
 *   the yogurt share node object
 */
$status       = _soyoco_view_node_fields($node,'status');
$week_date_fmt    = _soyoco_view_node_fields($node,'yogurt_week_date');
$week_date_datetime = $node->field_yogurt_week_date['und'][0]['value'];
$week_date_time = strtotime($week_date_datetime);
$week_date_fmt2 = date('D, M j', $week_date_time);
if (isset($node->field_primary_cook['und'][0]['target_id'])) {
  $primary_cook = $node->field_primary_cook['und'][0]['target_id'];
}
if (isset($node->field_joint_cooks['und'][0]['target_id'])) {
  $joint_cooks = $node->field_joint_cooks['und'];
}

$mode = 'signup';
$cook_params = array('primary' => FALSE, 'joint' => array());

$joint_cook_names = array();
if (isset($joint_cooks)) {
  // drupal_set_message(print_r($joint_cooks));
  foreach ($joint_cooks as $user_array) {
    $j = user_load(intval($user_array['target_id']));
    $joint_cook_names[] = $j->name;
    if ((int)$node->uid === (int)$user_array['target_id']) {
      $mode = 'cancel';
    }
  }
  if (sizeof($joint_cooks) > 0) {
    if (in_array($user->name, $joint_cook_names)) {
      $mode = 'update';
    } else {
      $mode = 'join';
    }
  }
}

if ($status === 'OPEN') {
  $label_type = 'success';
} else {
  $label_type = 'danger';
}

$cook_params = array(
  'joint'   => $joint_cook_names,
);

?>
<div id="node-<?php print $node->nid; ?>" class="work-shift row">
  <div class="span3">
    <h2><?php print $node->title; ?>
      <small>
        <?php print $week_date_fmt2; ?>   
      </small>
      <span class="label label-<?php print $label_type; ?>">
        <?php print $status; ?>
      </span>
    </h2>
  </div>
  <div class="span3">
    <?php print theme('soyoco_cooks', array('params' => $cook_params)); ?>
  </div>
  <div class="span2">
    <?php if($status != 'DONE' && $status != 'CLOSED'): ?>
      <?php if($mode == 'signup'): ?>
        <div class="btn-group">
          <a class="btn btn-mini btn-primary" href="/?q=node/<?php print $node->nid; ?>/edit/signup"><i class="icon-check icon-white"></i> sign up &raquo;</a>
        </div>
      <?php elseif($mode == 'join'): ?>
        <div class="btn-group">
          <a class="btn btn-mini" href="/?q=node/<?php print $node->nid; ?>/edit/join"><i class="icon-plus"></i> join shift &raquo;</a>
        </div>
      <?php elseif($mode == 'update'): ?>
        <div class="btn-group">
          <a class="btn btn-mini btn-danger" href="/?q=node/<?php print $node->nid; ?>/edit/update"><i class="icon-white icon-remove"></i> cancel</a>
          <a class="btn btn-mini" href="/?q=node/<?php print $node->nid; ?>/edit/update"><i class="icon-edit"></i> edit</a>
        </div>
      <?php endif; ?>
    <?php endif; ?>
  </div>
</div>
