<?php
/**
 * variables: $node (the yogurt share node object)
 * 
 * also available -- preprocessed in @see soyoco.module (hook_nodeapi)
 * 
 * $node->wjars_val  = soyoco_view_node_fields($node,'w_jars');
 * $node->njars_val  = soyoco_view_node_fields($node,'n_jars');
 * $node->duration   = soyoco_view_node_fields($node,'begin_date');
 * $node->end_date   = $node->field_begin_date['und'][0]['value2'];
 * $node->weeks_left = soyoco_weeks_til_expiry($end_date);
 */

$w_jars     = _soyoco_view_node_fields($node,'w_jars');
$n_jars     = _soyoco_view_node_fields($node,'n_jars');
$start_date = $node->field_start_date['und'][0]['value'];
$end_date   = _soyoco_view_node_fields($node,'end_date');
$start_date_time = strtotime($start_date);
$end_date_time = strtotime($end_date);

$start_date_fmt = date('M j, Y', $start_date_time);
$end_date_fmt = date('M j, Y', $end_date_time);

$weeks_left = _soyoco_view_node_fields($node,'weeks_left');
$wsd = _soyoco_view_node_fields($node,'work_share_difference');
$pct = _soyoco_view_node_fields($node,'completed_percentage');
$percent = $node->field_completed_percentage['und'][0]['value']; 

if ($percent > 100) {
  $cheer = 'Thanks for all the help!';
  $alert_type = 'success';
} else if ($percent > 0) {
  $cheer = 'Keep up the good work!';
  $alert_type = '';
} else {
  $cheer = 'Time to sign up!';
  $alert_type = 'info';
}
?>
<div class="alert alert-<?php print $alert_type; ?> large">
  <strong>
    <?php print $cheer; ?>
  </strong>&nbsp;
  Your work share is <strong><?php print $pct; ?></strong> complete.
</div>
<div id="node-<?php print $node->nid; ?>" class="yogurt-share row">
  <div class="span4 well" style="height:100px">
    <div class="row">
      <div class="span3">
        <?php for ($i = 0; $i < $w_jars; $i++): ?>
          <img src="/sites/all/themes/soyoco-bartik/images/ball-jar-small-wholefat.png" alt="Whole Fat Yogurt" title="Whole Fat Yogurt" />
        <?php endfor; ?>
        <?php for ($i = 0; $i < $n_jars; $i++): ?>
          <img src="/sites/all/themes/soyoco-bartik/images/ball-jar-small-nonfat.png" alt="Non Fat Yogurt" title="Non Fat Yogurt" />
        <?php endfor; ?>
      </div>
      <div class="span2">
        <p><strong>Whole Fats: </strong><?php print $w_jars; ?></p>
        <p><strong>Non Fats: </strong><?php print $n_jars; ?></p>
      </div>
    </div>
  </div>
  <div class="span4 well" style="height:100px; margin-right:-40px;">
    <div class="jumbotron">
      <h3><?php print $weeks_left; ?> <small>weeks left</small></h3>
    </div>
    <div style="color:#333">
      <dl>
        <dt>Share Duration:</dt>
        <dd><?php print $start_date_fmt; ?> - <?php print $end_date_fmt; ?></dd>
      </dl>
    </div>
  </div>
</div>
<h2>Upcoming Work Shifts</h2>
