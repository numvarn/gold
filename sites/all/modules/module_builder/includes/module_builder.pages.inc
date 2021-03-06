<?php
// $Id: module_builder.pages.inc,v 1.1.2.5 2009/10/24 13:27:18 joachim Exp $

/**
 * @file
 *   Menu callback for main module builder page.
 *
 */

/**
 * @defgroup module_builder_callback Functions which are the menu callbacks for this module
 */

/**
 * Displays module builder interface via a multi-step form.
 * The steps are:
 *
 * - input    => shows a form where the user can enter module options.
 * - module   => shows the generated module and info files.
 * - download => pushes a file for download.
 * - write    => writes files.
 *
 * @ingroup module_builder_callback
 * @param $form_values will be NULL when the page is first displayed,
 *   when the form is submitted, this will be an array of the submitted
 *   values.
 * @return
 *   One of three results depending on the state of this multi-step form.
 *   Form for entering module options
 *   Form showing built module and info file
 *   Nothing, but file is pushed to client for download
 */
  # fun fun fun!
  # http://drupal.org/node/144132
function module_builder_page($form_state) {
  $step = 'input';
  if (isset($form_state['clicked_button'])) {
     $step = $form_state['clicked_button']['#name'];
  }
  #dsm($step);
  switch ($step) {
    case 'input':
      // sanity check first time around
      _module_builder_check_settings();
      // fall through
    case 'back':
      $form = module_builder_page_input($form_state);
      break;
    case 'generate':
      $form = module_builder_page_generate($form_state);
      break;
    case 'download':
      $form = module_builder_page_download($form_state);
      break;
  }

  return $form;
}

// build page 1
function module_builder_page_input($form_state) {
  /*
  // built some default values.
  // these are either hardcoded or what the user already put into the form on a previous go round.
  $form_default_values = array(
    'module_root_name' => 'mymodule',
  );
  if (isset($form_state['storage']['input'])) {
    $form_default_values = array_merge($form_default_values, $form_state['storage']['input']);
  }

  #dsm($form_default_values);
  #dsm($form_state['storage']['input']);
  dsm($form_state);
  */

  // sanity check first time around
  #_module_builder_check_settings(); // moved up to main form builder
  // Get list of hooks from downloaded documentation, organized in fieldsets.
  // include the data processing file
  // TODO: only need the data loading.
  module_builder_include('process');
  
  $hook_groups = module_builder_get_hook_data();
  if (!is_array($hook_groups) || !count($hook_groups)) {
    form_set_error('hooks', t('No hooks were found. Please check the documentation path specified in the <a href="!settings">%administer >> %settings >> %modulebuilder</a> page.', array('!settings' => url('admin/settings/module_builder'), '%administer' => 'Administer', '%settings' => 'Site configuration', '%modulebuilder' => "Module builder")));
    return $form;
  }

  // Include CSS for formatting
  drupal_add_css(drupal_get_path('module', 'module_builder') . '/theme/module_builder.css');

  // Mark form as fresh to enable JS clearing of fields with sample text.
  if ($form_state['clicked_button']['#name'] != 'back') {
    $form['#attributes'] = array('class' => 'fresh');
  }

  // Module properties
  $form['module_root_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine-readable name'),
    '#description' => t('This string is used to name the module files and to prefix all of your functions. This must only contain letters, numbers, and underscores, and may not start with a number.'),
    '#required' => TRUE,
    '#default_value' => t('mymodule'), # $form_default_values['module_root_name'],
    '#repopulate' => TRUE,
  );
  $form['module_readable_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name of your module as it will appear on the module admin page.'),
    '#required' => TRUE,
    '#default_value' => t('My Module'),
    '#repopulate' => TRUE,
  );
  $form['module_short_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('This text will appear in the module listing at <a href="!listing">%administer >> %build >> %modules</a>.', array('!listing' => url('admin/build/modules'), '%administer' => 'Administer', '%build' => 'Site building', '%modules' => 'Modules')),
    '#required' => TRUE,
    '#default_value' => t('Does awesome things. Makes tea. Washes up. Favours of a personal nature.'),
    '#repopulate' => TRUE,
  );
  $form['module_help_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#description' => t('Help text (HTML) to appear in <a href="!help">%administer >> %help >> module_name</a> page.', array('!help' => url('admin/help'), '%administer' => 'Administer', '%help' => 'Help')),
    '#repopulate' => TRUE,
  );
  $form['module_dependencies'] = array(
    '#type' => 'textfield',
    '#title' => t('Dependencies'),
    '#description' => t('Space seperated list of other modules that your module requires.'),
    '#repopulate' => TRUE,
  );
  $form['module_package'] = array(
    '#type' => 'textfield',
    '#title' => t('Package'),
    '#description' => t('If your module comes with other modules or is meant to be used exclusively with other modules, enter the package name here. Suggested package names: Audio, Bot, CCK, Chat, E-Commerce, Event, Feed parser, Organic groups, Station, Video, Views and Voting.'),
    '#repopulate' => TRUE,
  );

  // Check for custom hook_groups file, else use default
  $path = drupal_get_path('module', 'module_builder');
  if (file_exists($path . MODULE_BUILDER_CUSTOM_HOOK_GROUPS_TEMPLATE_PATH)) {
    $template_file = file_get_contents($path . MODULE_BUILDER_CUSTOM_HOOK_GROUPS_TEMPLATE_PATH);
  }
  else {
    $template_file = file_get_contents($path . MODULE_BUILDER_HOOK_GROUPS_TEMPLATE_PATH);
  }

  $form['hook_groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hook groupings'),
    '#description' => t('Selecting one or more of these features will automatically select appropriate hooks for you.'),
  );

  drupal_add_js($path . '/theme/module_builder.js');

  // Get list of hook presets from installed template.
  // Include generating component file.
  module_builder_include('process');
  $hook_presets = module_builder_parse_template($template_file);
  foreach ($hook_presets as $hook_preset_name => $hook_preset) {
    $hooks = explode("\n", $hook_preset['template']);
    $hook_array = array();
    foreach ($hooks as $hook) {
      $hook = trim($hook);
      if (!empty($hook)) {
        $hook_array[] = "'$hook'";
      }
    }

    $form['hook_groups']['groups-'. $hook_preset_name] = array(
      '#type' => 'checkbox',
      '#title' => $hook_preset_name,
      '#attributes' => array(
        'onclick' => 'check_hooks(this, ['. implode(', ', $hook_array) .'])',
      ),
      //'#description' => $hook_groups[$i]['data'],

      // TODO: For some reason this gives me some wacky error under PHP 5:
      // Fatal error: Cannot use string offset as an array
      //'#default_value' => $edit['hook_groups'][$i],
    );
  }


  // Build hooks list
  $form['hooks'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Use the following specific hooks'),
   // '#description' =>  '(Note that for now, only core and node hooks will be generated: for the others use drush.)',
  );

  foreach ($hook_groups as $hook_group => $hooks) {
    $form['hooks'][$hook_group] = array(
      '#type' => 'fieldset',
      '#title' => $hook_group .' hooks',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#theme'  => 'module_builder_hook_list',
    );
    foreach ($hooks as $hook) {
      $name = $hook['name'];
      $desc = $hook['description'];
      $form['hooks'][$hook_group][$name] = array(
        '#type' => 'checkbox',
        '#title' => preg_replace('/^hook_/', '', $name),
        '#description' => $desc,
        '#repopulate' => TRUE,


        // TODO: For some reason this gives me some wacky error under PHP 5:
        // Fatal error: Cannot use string offset as an array
        //'#default_value' => $edit['hooks'][$hook_group][$hook],

        // TODO: I would *like* to do something like the following, so some of the long
        // descriptions don't totally mangle the page output, but need a way to do like
        // a "onmouseover" effect to display the full thing. Note that 'title' is not
        // a valid attribute for divs. :\
        //'#description' => truncate_utf8($desc, 40, TRUE, TRUE),

      );
      // Set some default hooks
      if ($name == 'hook_menu') {
        $form['hooks'][$hook_group][$name]['#default_value'] = 1;
      }
    }
    // Sort list alphabetically
    ksort($form['hooks'][$hook_group]);
  }

  $form['generate_module'] = array(
    '#type' => 'submit',
    '#name' => 'generate',
    '#value' => t('Generate'),
  );
  $form['#submit'] = array('module_builder_page_input_submit');

  #if ($form_state['rebuild']) { // fails as a test!?!?!
  if ($form_state['values']) {
    #dsm('rebuild');
    $form = _form_repopulate($form, $form_state);
    #dsm($form_state['storage']['input']);
  }

  return $form;
}

/**
 * Repopulate form with user values.
 */
function _form_repopulate($form, $form_state) {
  #dsm($form);
  #dsm(element_children($form));
  #dsm($form_state);
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#repopulate'])) {
      #dsm('repop: ');
      #dsm($key);
      #$form[$key]['#default_value'] = 'repop!'; // this obviously works
      #$form[$key]['#default_value'] = $form_state['values'][$key]; // arg! here we only have values from page 2!
      $form[$key]['#default_value'] = $form_state['storage']['input'][$key]; // this obviously works
    }
    // recurse into children
    $form[$key] = _form_repopulate($form[$key], $form_state);
  } // each element_children
  return $form;
}


/**
 * Theme function for hook list
 */
function theme_module_builder_hook_list($form) {
  $output = "<ul class=\"hook-group clear-block\">\n";
  foreach (element_children($form) as $key) {
    $output .= "  <li>". drupal_render($form[$key]) ."</li>\n";
  }
  $output .= "</ul>\n";
  return $output;
}


/**
 * submit page 1
 */
function module_builder_page_input_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  // stash input.... these values will follow us around everywhere like a little dog...
  $form_state['storage']['input'] = $form_state['values'];
  foreach (array('generate', 'generate_module', 'form_build_id', 'form_token', 'form_id') as $key) {
    unset($form_state['storage']['input'][$key]);
  }
}

/**
 * page 2: generate code
 */
function module_builder_page_generate($form_state) {
  module_builder_include('generate');
  
  // Build module data
  $module_data = module_data_from_form($form_state['values']);

  $code_array = module_builder_generate_module($module_data);
  //dsm($code_array);

  /*
  // the restitution was all for the DL buttons, no?
  // these should be ajaxy!
  if ($form_state['values']['module_code']) {
    $code = $form_state['values']['module_code'];
  }
  else {
    // Bit messy. This returns an array now.
    // TODO: the other files!
    $code_array = module_builder_generate_module($module_data);

    $code = $code_array[$module_data['module_root_name'] . '.module'];
  }
  */
  module_builder_include('generate_info');  
  $info = $form_state['values']['module_info'] ? $form_state['values']['module_info'] : module_builder_generate_info($module_data);
    // damn I miss perl at times like this. fugly syntax.

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#name' => 'back',
  );

  foreach ($code_array as $filename => $code) {
    $form['code_instructions_' . $filename] = array(
      '#value' => t('Please copy and paste the following text into a file called !module.', array('!module' => $filename)),
      '#prefix' => '<div class="module-message">',
      '#suffix' => '</div>',
    );
    $form['module_code_'  . $filename] = array(
      '#type' => 'textarea',
      '#title' => t($filename .' code'),
      '#rows' => 20,
      '#default_value' => $code,
      '#prefix' => '<div class="module-code">',
      '#suffix' => '</div>',
    );
  }

  /*
  $form['code_instructions'] = array(
    '#value' => t('Please copy and paste the following text into a file called !module.', array('!module' => $form_state['values']['module_root_name'] .'.module')),
    '#prefix' => '<div id="module-message">',
    '#suffix' => '</div>',
  );
  $form['module_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Module code'),
    '#rows' => 20,
    '#default_value' => $code,
    '#prefix' => '<div id="module-code">',
    '#suffix' => '</div>',
  );
  */
  /*
  $form['download_module'] = array(
    '#type' => 'submit',
    '#value' => t('Download module'),
  );
  $form['write_module'] = array(
    '#type' => 'button',
    '#value' => t('Write module file'),
  );
  */

  $form['info_instructions'] = array(
    '#value' => t('Please copy and paste the following text into a file called !module.', array('!module' => $form_state['values']['module_root_name'] .'.info')),
    '#prefix' => '<div id="module-message">',
    '#suffix' => '</div>',
  );
  $form['module_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Module info'),
    '#rows' => 20,
    '#default_value' => $info,
    '#prefix' => '<div id="module-info">',
    '#suffix' => '</div>',
  );
  /*
  $form['download_info'] = array(
    '#type' => 'submit',
    '#name' => 'op',
    '#value' => t('Download info file'),
  );
  $form['write_info'] = array(
    '#type' => 'button',
    '#value' => t('Write info file'),
  );
  */

  // handle the write buttons
 ## $form['#after_build'] = array('module_builder_write_buttons');

  return $form;
}

/**
 * Helper function: creates an array of all the data needed to build the module
 * from form values, suitable for passing to module_builder_generate_module().
 */
function module_data_from_form($form_values) {
  // Most things come in as we want them from the form.
  $data = $form_values;

  // Hooks need flattening.
  $data['hooks'] = array();
  foreach ($form_values['hooks'] as $hook_group) {
    $data['hooks'] += $hook_group;
  }

  return $data;
}
