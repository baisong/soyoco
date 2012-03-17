<?php
/**
 * variables: $node (the yogurt share node object)
 */
$status       = _soyoco_view_node_fields($node,'status');
$week_date    = _soyoco_view_node_fields($node,'yogurt_week_date');
if (isset($node->field_primary_cook['und'][0]['target_id'])) {
  $primary_cook = $node->field_primary_cook['und'][0]['target_id'];
}
if (isset($node->field_joint_cooks['und'][0]['target_id'])) {
  $joint_cooks = $node->field_primary_cook['und'];
}
$mode = 'signup';
global $user;
if (isset($primary_cook)) {
  if ((int)$user->uid === (int)$primary_cook) {
    $mode = 'cancel';
  }
}
if (isset($joint_cooks)) {
  foreach ($joint_cooks as $cook=>$target_id) {
    if ((int)$user->uid === (int)$target_id) {
      $mode = 'cancel';
    }
  }
}
if ($status === 'OPEN') {
  $label_type = 'success';
} else {
  $label_type = 'danger';
}

?>
<div id="node-<?php print $node->nid; ?>" class="work-shift row">
  <div class="span4">
    <h2><?php print $node->title; ?>
      <small><?php print $week_date; ?></small>
    </h2>
  </div>
  <div class="span3">
    <span class="label label-<?php print $label_type; ?>">
      <?php print $status; ?>
    </span>
    &nbsp;
    <?php if($status != 'DONE' && $status != 'CLOSED'): ?>
      <?php if($mode == 'signup'): ?>
        <a href="/?q=node/<?php print $node->nid; ?>/edit">sign up &raquo;</a>
      <?php elseif($mode == 'cancel'): ?>
        <a href="/?q=node/<?php print $node->nid; ?>/edit/cancel">cancel &raquo;</a>
      <?php endif; ?>
    <?php endif; ?>
  </div>
</div>
