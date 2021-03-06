<?php
// $Id: generate_info_7.inc,v 1.1.2.6 2011/01/26 20:17:09 joachim Exp $

/**
 * @file
 *   Module builder code generating code specific to Drupal 7.
 */

/**
 * Generate module info file code.
 *
 * @param $module_data
 *   Same as for generate_module().
 *   An associative array of data for the module. The keys can *mostly* be taken
 *   straight from form values. They are as follows:
 *     - 'module_root_name'
 *     - 'module_readable_name'
 *     - 'module_short_description'
 *     - 'module_help_text'
 *     - 'hooks': An associative array whose keys are full hook names
 *       (eg 'hook_menu'), where requested hooks have a value of TRUE.
 *       Unwanted hooks may also be included as keys provided their value is FALSE.
 *     - 'module_dependencies': a string of dependencies, eg 'forum views'.
 *     - 'module_package': the module package.
 */
function module_builder_generate_info($module_data) {
  //print_r($module_data);

  // The weird syntax stops this from getting mangled by CVS
  $info = '; $' . 'Id$' . "\n";
  $info .= 'name = ' . $module_data['module_readable_name'] . "\n";
  $info .= 'description = '. $module_data['module_short_description'] ."\n";

  if (!empty($module_data['module_dependencies'])) {
    foreach (explode(' ', $module_data['module_dependencies']) as $dep) {
      $info .= 'dependencies[] = '. $dep ."\n";
    }
  }

  if (!empty($module_data['module_package'])) {
    $info .= 'package = '. $module_data['module_package'] ."\n";
  }
  $info .= "core = 7.x\n";
  
  if (isset($module_data['module_files'])) {
    foreach ($module_data['module_files'] as $file) {
      $info .= 'files[] = '. $file ."\n";      
    }
  }

  return $info;
}

