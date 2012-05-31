<?php
/**
 * @file soyoco.node.inc
 * Drupal 7 module node and node form hooks
 */


/**
 * implements HOOK_form_FORM_ID_alter()
 */
function soyoco_form_work_shift_node_form_alter(&$form, &$form_state) {
  if (user_access('view administration menu')) { dpm($form); return; }
  global $user;
  
  $user_action = arg(3);
  
  if (isset($form['#node']->field_yogurt_week_date['und'][0]['value'])) {
    $week_date = $form['#node']->field_yogurt_week_date['und'][0]['value'];
    $short_date = date("F jS", strtotime($week_date));
    $long_date = date("l, F jS, Y", strtotime($week_date));
    $form['title']['#prefix'] = '
        <span class="large">Make yogurt for the week of <strong>' . $long_date . '</strong></span>
    ';
  }

  $form['actions']['submit']['#suffix'] = '<a href="/?q=user">Cancel</a>';
  if ($user_action == 'signup') {
    global $user;
    drupal_set_title('Sign up for the ' . $form['#node']->title . ' shift');  
    $user_id = $user->name . ' (' . $user->uid . ')';
    $form['field_cook_date']['#prefix'] = '<div class="alert alert-info margin-top-05em margin-bottom-4em"><strong>Thanks for signing up!</strong> Please confirm what time you plan to cook.<br/></div>';
    $form['field_primary_cook']['und'][0]['target_id']['#attributes']['class'][] = 'display-none';
    if (empty($form['field_primary_cook']['und'][0]['target_id']['#default_value'])) {
      $form['field_primary_cook']['und'][0]['target_id']['#default_value'] = $user_id;
      // $form['field_joint_cooks']['#attributes']['class'][] = 'display-none';
    } else {
      $delta = count($form['field_joint_cooks']['und']);
      // $form['field_joint_cooks']['und'][$delta]['target_id']['#default_value'] = $user->uid;
    }
    $form['#action'] .= '?destination=user';
    // $form['actions']['submit']['#submit'][] = 'soyoco_shift_submit';
    $form['field_joint_cooks']['und']['#title'] = 'Your jar name (do not change)';
    $form['field_joint_cooks']['und'][0]['target_id']['#default_value'] = $user->name;
    $form['actions']['submit']['#value'] = 'Sign up';
    return;
  }
  if ($user_action == 'join') {
    global $user;
    drupal_set_title('Join the ' . $form['#node']->title . ' shift');
    $user_id = $user->name . ' (' . $user->uid . ')';
    $cook_datetime = $form['field_cook_date']['und'][0]['#default_value']['value'];
    $datepart_fmt = date("l, F jS", strtotime($cook_datetime . ' GMT'));
    $timepart_fmt = date("g:i A", strtotime($cook_datetime . ' GMT'));
    $datetime_fmt = $datepart_fmt . ' at ' . $timepart_fmt;
    $form['field_cook_date']['#prefix'] = '<div class="alert alert-info margin-top-05em">
        <strong>Welcome aboard!</strong> Yogurt making is scheduled for ' . $datetime_fmt . '.<br/></div>';
    $form['#action'] .= '?destination=user';
    // $form['actions']['submit']['#submit'][] = 'soyoco_shift_submit';
    $form['field_joint_cooks']['und']['#title'] = 'Your jar name (do not change)';
    $cooks_count = $form['#node']->field_cooks['und'][0]['value'];
    $form['field_joint_cooks']['und'][$cooks_count]['target_id']['#default_value'] = $user->name . ' (' . $user->uid . ')';
    $form['actions']['submit']['#value'] = 'Join this shift';
    return;
  }
  if ($user_action == 'cancel') {
    drupal_set_title('Leave the' . $form['#node']->title . ' shift');
    $form['title']['#prefix'] = '
        <span class="large">Make yogurt for the week of <strong>' . $long_date . '</strong></span>';
    $form['field_cook_date']['#prefix'] = '<div class="alert margin-top-05em">
        <strong>Are you sure don\'t want to cook this night?</strong></div>';
    $form['actions']['submit']['#value'] = 'Leave this shift';
    return;
  }
  if ($user_action == 'update') {
    $form['field_cook_date']['#prefix'] = '<div class="alert margin-top-05em margin-bottom-4em">
        <strong>Experimental!</strong> Here you can change the date, and member participation. To cancel, delete your name if it appears in the jar name list, then hit "Save".</div>';
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
