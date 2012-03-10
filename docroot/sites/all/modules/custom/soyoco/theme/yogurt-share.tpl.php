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
?>
<div id="node-<?php print $node->nid; ?>" class="yogurt-share row">
  <div class="span5 well" style="height:100px">
    <div class="row">
      <div class="span3">
        <?php for ($i = 0; $i < $node->wjars_val; $i++): ?>[W]<?php endfor; ?>
        <?php for ($i = 0; $i < $node->njars_val; $i++): ?>[N]<?php endfor; ?>
      </div>
      <div class="span2">
        <p><strong>Whole Fats:</strong><?php print $node->wjars_val; ?></p>
        <p><strong>Non Fat:</strong><?php print $node->njars_val; ?></p>
      </div>
    </div>
  </div>
  <div class="span3 well" style="height:100px">
    <div class="jumbotron">
      <h1><?php print $node->weeks_left; ?><small>weeks left</small></h1>
    </div>
    <div style="color:#333">
      <dl>
        <dt>Share Duration:</dt>
        <dd><?php print $node->duration; ?></dd>
      </dl>
    </div>
  </div>
</div>
