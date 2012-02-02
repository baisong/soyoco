<?php
/**
 * @file
 * yogurt_shares.features.inc
 */

/**
 * Implements hook_node_info().
 */
function yogurt_shares_node_info() {
  $items = array(
    'yogurt_share' => array(
      'name' => t('Yogurt Share'),
      'base' => 'node_content',
      'description' => t('Represents a single share of yogurt. One person may have multiple simultaneous overlapping shares.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_profile2_type().
 */
function yogurt_shares_default_profile2_type() {
  $items = array();
  $items['main'] = entity_import('profile2_type', '{
    "userCategory" : true,
    "userView" : true,
    "type" : "main",
    "label" : "Main profile",
    "weight" : "0",
    "data" : { "registration" : true, "use_page" : true },
    "rdf_mapping" : [  ]
  }');
  return $items;
}
