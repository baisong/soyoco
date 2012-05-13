<?php

/**
 * @file soyoco.node.inc
 * Drupal 7 module node and node form hooks
 */


/**
 * implements hook_form_FORM_ID_alter()
 *
 */
function soyoco_form_work_shift_node_form_alter(&$form, &$form_state, $form_id) {
  // dpm($form);
  // @TODO:
  //   * make joint cook thingie automatic
  //   * hide both primary and joint as appropriate
  //   * make cancel work
  
  if (isset($form['#node']->field_yogurt_week_date['und'][0]['value'])) {
    $week_date = $form['#node']->field_yogurt_week_date['und'][0]['value'];
    $form['title']['#prefix'] = 'For the week of ' . date("l, F jS, Y", strtotime($week_date));
  }
  unset($form['actions']['preview']);
  $form['actions']['submit']['#suffix'] = '<a href="/?q=user">Cancel</a>';
  $form['actions']['submit']['#value'] = 'Confirm Work Shift';
  if (arg(3) == 'signup') {
    drupal_set_message('Signing up!');
    global $user;
    $user_id = $user->name . ' (' . $user->uid . ')';
    $form['field_primary_cook']['und'][0]['target_id']['#attributes']['class'][] = 'display-none';
    if (empty($form['field_primary_cook']['und'][0]['target_id']['#default_value'])) {
      $form['field_primary_cook']['und'][0]['target_id']['#default_value'] = $user_id;
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
