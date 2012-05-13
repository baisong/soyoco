<?php
/**
 *
 *
 */

/**
 * Implements HOOK_theme
 * [or] 2012-05-13
 */
function soyoco_theme() {
  $tpl_path = drupal_get_path('module', 'soyoco') . '/theme';
  return array(
    'soyoco_yogurt_share' => array(
      'variables' => array('node' => NULL),
      'template' => 'yogurt-share',
      'path' => $tpl_path,
    ),
    'soyoco_work_shift' => array(
      'variables' => array('node' => NULL),
      'template' => 'work-shift',
      'path' => $tpl_path,
    ),
    'soyoco_cooks' => array(
      'variables' => array('params' => NULL),
      'template' => 'cooks',
      'path' => $tpl_path,
    ),
    'soyoco_admin_dash' => array(
      'variables' => array(),
      'template' => 'admin-dash',
      'path' => $tpl_path,
    ),
  );
}


/**
 * Provides the output for the menu item at 'admin/dash'
 * @see soyoco_menu
 * [or] 2012-05-12
 */
function soyoco_admin_dash_view() {
  return theme('soyoco_admin_dash');
}
