<?php 
/**
 * Arguments
 * $params
 *   An associative array containing primary & joint user UIDs.
 * -'primary' the primary cook user UID, or 0 if none.
 * -'joint' an array of uids, or an empty array if none.
 */

$count_cooks = count($params['joint']);
$i = 0;
?>
<span class="cooks">
  <?php if($count_cooks > 0): ?>
    <?php foreach($params['joint'] as $name): ?>
      <?php print $name; ?><?php $i++; if ($i < $count_cooks) { print ', ';} ?>
    <?php endforeach; ?>
  <?php else: ?>
    &nbsp;
  <?php endif; ?>
</span>
