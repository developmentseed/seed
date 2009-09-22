<?php

/**
 * Implementation of hook_views_handlers().
 */
function seed_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'seed') .'/views',
    ),
    'handlers' => array(
      'seed_handler_field_date' => array(
         'parent' => 'views_handler_field_date',
       ),
    ),
  );
}

/**
 * Implementation of hook_views_data_alter().
 * Replaces default handlers with custom handlers.
 */
function seed_views_data_alter(&$cache) {
  if (variable_get('seed_views_overrides', TRUE)) {
    foreach ($cache as $module => $a) {
      foreach ($a as $column => $b) {
        foreach ($b as $property => $c) {
          if ($property == 'field' && !empty($c['handler'])) {
            switch ($c['handler']) {
              case 'views_handler_field_date':
                $cache[$module][$column][$property]['handler'] = 'seed_handler_field_date';
                break;
            }
          }
        }
      }
    }
  }
}
