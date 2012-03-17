<?php

/**
 * @file soyoco.node.inc
 * Drupal 7 module node and node form hooks
 */


/**
 * implements hook_node_insert()
 */
function soyoco_node_insert($node) {
  
 /*
 // @TODO auto-create missing work-shifts
 //
 // when you save a yogurt share node,
 // compare it's end date to var:final-work-shift-date
 // if var < node->date2,
 //    foreach week in period {
 //        create a new work shift node
 //    }
 //    update var
 */
}


/**
 * implements hook_node_update()
 */
function soyoco_node_update($node) {
 // @see soyoco_node_insert()
}

/**
 * implements hook_form_FORM_ID_alter()
 *
 */
function soyoco_form_work_shift_node_form_alter(&$form, &$form_state, $form_id) {
 /*
 // @TODO hide primary_cook fld from non-admins if already set to non-curr usr
 //
  ($form_id)
    if !(user_access()) {
      var 
      var 
      if (!(empty(field) && val != usr
        $form['primary_cook']['#access'] = FALSE
 */
}

/**
 * implements hook_node_load()
 *
 *
 */
function soyoco_node_load($nodes,$types) {
  $soyoco_types = array('yogurt_share');
  foreach ($types as $type) {
    if (in_array($type,$soyoco_types)) { $soyoco_types[] = $type; }
  }
  // return if we're not dealing with any custom nodes
  if (!count($soyoco_types)) { return; }
  
  foreach ($nodes as $node) {
    switch($node->type) {
      case 'yogurt_share':
         break;
      case 'work_shift':
         break;
    }
  }
}
