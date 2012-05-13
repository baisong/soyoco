<?php 
/**
 * Arguments
 * $params
 *   An associative array containing primary & joint user UIDs.
 * -'primary' the primary cook user UID, or 0 if none.
 * -'joint' an array of uids, or an empty array if none.
 */
 // dpm($params);
?>
<span class="cooks">
  &nbsp;<strong><?php print $params['primary']; ?></strong>
  <?php if(count($params['joint'])): ?>
    <?php foreach($params['joint'] as $name): ?>
      , <?php print $name; ?>
    <?php endforeach; ?>
  <?php endif; ?>
</span>
