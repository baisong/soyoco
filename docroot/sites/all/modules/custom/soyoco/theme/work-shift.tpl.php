<?php
/**
 * variables:
 * $node 
 *   the yogurt share node object
 */
$status       = _soyoco_view_node_fields($node,'status');
$week_date    = _soyoco_view_node_fields($node,'yogurt_week_date');

if (isset($node->field_primary_cook['und'][0]['target_id'])) {
  $primary_cook = $node->field_primary_cook['und'][0]['target_id'];
}
if (isset($node->field_joint_cooks['und'][0]['target_id'])) {
  $joint_cooks = $node->field_joint_cooks['und'];
}

$mode = 'signup';
$cook_params = array('primary' => FALSE, 'joint' => array());

$primary_cook_name = '';
if (isset($primary_cook)) {
  $u = user_load(intval($primary_cook));
  $primary_cook_name = $u->name;
  $mode = 'join shift';
  if ((int)$node->uid === (int)$primary_cook) {
    $mode = 'cancel';
  }
}

$joint_cook_names = array();
if (isset($joint_cooks)) {
  foreach ($joint_cooks as $target_id) {
    $j = user_load(intval($target_id['target_id']));
    $joint_cook_names[] = $j->name;
    if ((int)$node->uid === (int)$target_id['target_id']) {
      $mode = 'cancel';
    }
  }
}

if ($status === 'OPEN') {
  $label_type = 'success';
} else {
  $label_type = 'danger';
}

$cook_params = array(
  'primary' => $primary_cook_name,
  'joint'   => $joint_cook_names,
);

?>
<div id="node-<?php print $node->nid; ?>" class="work-shift row">
  <div class="span4">
    <h2><?php print $node->title; ?>
      <small>
        <?php print $week_date; ?>   
      </small>
      <span class="label label-<?php print $label_type; ?>">
        <?php print $status; ?>
      </span>
    </h2>
  </div>
  <div class="span2">
    <?php print theme('soyoco_cooks',array('params' => $cook_params)); ?>
  </div>
  <div class="span2">
    <?php if($status != 'DONE' && $status != 'CLOSED'): ?>
      <?php if($mode == 'signup'): ?>
        <div class="btn-group">
          <a class="btn btn-mini btn-primary" href="/?q=node/<?php print $node->nid; ?>/edit/signup"><i class="icon-check icon-white"></i> sign up &raquo;</a>
        </div>
      <?php elseif($mode == 'join shift'): ?>
        <div class="btn-group">
          <a class="btn btn-mini" href="/?q=node/<?php print $node->nid; ?>/edit/signup"><i class="icon-plus"></i> join shift &raquo;</a>
        </div>
      <?php elseif($mode == 'cancel'): ?>
        <div class="btn-group">
          <a class="btn btn-mini btn-danger" href="/?q=node/<?php print $node->nid; ?>/edit/cancel"><i class="icon-white icon-remove"></i> cancel</a>
          <a class="btn btn-mini" href="/?q=node/<?php print $node->nid; ?>/edit"><i class="icon-edit"></i> edit</a>
        </div>
      <?php endif; ?>
    <?php endif; ?>
  </div>
</div>
