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
$start_date = _soyoco_view_node_fields($node,'start_date');
$end_date   = _soyoco_view_node_fields($node,'end_date');
$weeks_left = _soyoco_view_node_fields($node,'weeks_left');
$wsd = _soyoco_view_node_fields($node,'work_share_difference');
$pct = _soyoco_view_node_fields($node,'completed_percentage');
?>
<div class="alert alert-info">
  Your current Work Share Difference is <strong><?php print $wsd; ?></strong> (<?php print $pct; ?>).
</div>
<div id="node-<?php print $node->nid; ?>" class="yogurt-share row">
  <div class="span4 well" style="height:100px">
    <div class="row">
      <div class="span3">
        <?php for ($i = 0; $i < $w_jars; $i++): ?>[W]<?php endfor; ?>
        <?php for ($i = 0; $i < $n_jars; $i++): ?>[N]<?php endfor; ?>
      </div>
      <div class="span2">
        <p><strong>Whole Fats:</strong><?php print $w_jars; ?></p>
        <p><strong>Non Fat:</strong><?php print $n_jars; ?></p>
      </div>
    </div>
  </div>
  <div class="span3 well" style="height:100px">
    <div class="jumbotron">
      <h3><?php print $weeks_left; ?> <small>weeks left</small></h3>
    </div>
    <div style="color:#333">
      <dl>
        <dt>Share Duration:</dt>
        <dd><?php print $start_date; ?> - <?php print $end_date; ?></dd>
      </dl>
    </div>
  </div>
</div>
