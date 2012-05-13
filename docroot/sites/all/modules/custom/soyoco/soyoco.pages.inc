<?php
/**
 *
 *
 */

/**
 * Implements HOOK_menu
 * [or] 2012-05-13
 */
function soyoco_menu() {
  $items['admin/dash'] = array(
    'title' => 'Admin Dash',
    'page callback' => 'soyoco_admin_dash_view',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'access arguments' => array('administer content'),
  );
  return $items;
}

/**
 * Provides the output for the menu item at 'admin/dash'
 * @see soyoco_menu
 * [or] 2012-05-12
 */
function soyoco_admin_dash_view() {
  return theme('soyoco_admin_dash');
}
