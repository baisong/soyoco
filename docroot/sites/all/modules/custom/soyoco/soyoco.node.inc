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
  dpm($form);
  $week_date = $form['#node']->field_yogurt_week_date['und'][0]['value'];
  $form['title']['#prefix'] = 'For the week of ' . date("l, F jS, Y",strtotime($week_date));
  unset($form['actions']['preview']);
  $form['actions']['submit']['#suffix'] = '<a href="/?q=user">Cancel</a>';
  $form['actions']['submit']['#value'] = 'Confirm Work Shift';
  if (arg(3) == 'signup') {
    global $user;
    $form['field_primary_cook']['und'][0]['target_id']['#attributes']['class'][] = 'display-none';
    if (empty($form['field_primary_cook']['und'][0]['target_id']['#default_value'])) {
      $form['field_primary_cook']['und'][0]['target_id']['#default_value'] = $user->uid;
      // $form['field_joint_cooks']['#attributes']['class'][] = 'display-none';
    } else {
      $delta = count($form['field_joint_cooks']['und']);
      // $form['field_joint_cooks']['und'][$delta]['target_id']['#default_value'] = $user->uid;
    }
    $form['#action'] .= '?destination=thank-you';
    // $form['actions']['submit']['#submit'][] = 'soyoco_shift_submit';
    return;
  }
  if (arg(3) == 'cancel') {
    if ($form['field_primary_cook']['und'][0]['target_id']['#default_value'] == $user->uid) {
      $form['field_primary_cook']['und'][0]['target_id']['#default_value'] = 0;
      $form['field_primary_cook']['#attributes']['class'][] = 'display-none';
    } else {
    
    }
    $form['#action'] .= '?destination=thank-you';
    //$form['actions']['submit']['#submit'][] = 'soyoco_shift_submit';
    return;
  }
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

function soyoco_shift_submit($form, &$form_state) {
  drupal_set_message($form['form_id']);
  $form_state['rebuild'] = TRUE;
  $form_state['redirect'] = 'user';
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
