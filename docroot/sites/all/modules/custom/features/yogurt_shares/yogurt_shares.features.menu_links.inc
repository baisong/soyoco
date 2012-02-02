<?php
/**
 * @file
 * yogurt_shares.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function yogurt_shares_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:<front>
  $menu_links['main-menu:<front>'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Home',
    'options' => array(),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: main-menu:https://github.com/baisong/soyoco
  $menu_links['main-menu:https://github.com/baisong/soyoco'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'https://github.com/baisong/soyoco',
    'router_path' => '',
    'link_title' => 'Source Code',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Home');
  t('Source Code');


  return $menu_links;
}
