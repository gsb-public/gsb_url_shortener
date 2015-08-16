<?php

/**
 * Page callback for the shortened url overview.
 */
function gsb_url_shortener_overview() {
  $build = array();

  $header = array(
    'alias' => array('data' => t('Alias'), 'field' => 'alias'),
    'destination' => array('data' => t('Destination'), 'field' => 'destination'),
    'operations' => array('data' => t('Operations')),
  );

  $links = \GSBUrlShortener::loadFromPager($header);
  $rows = array();
  foreach ($links as $link) {
    $row = array();
    $row['alias'] = l($link->alias, $link->alias);
    $row['destination'] = l($link->destination, $link->destination);

    $operations['edit'] = array(
      'title' => t('Edit'),
      'href' => 'admin/config/gsb/url-shortener/manage/' . $link->id . '/edit',
    );
    $operations['delete'] = array(
      'title' => t('Delete'),
      'href' => 'admin/config/gsb/url-shortener/manage/' . $link->id . '/delete',
    );
    $row['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );
    $rows[] = $row;
  }

  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No shortened urls available.'),
  );
  $build['pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Page callback for adding a new shortened url.
 */
function gsb_url_shortener_add_page() {
  drupal_set_title(t('Add new shortened url'));
  $link = new \GSBUrlShortener();
  return drupal_get_form('gsb_url_shortener_form', $link);
}

/**
 * Form constructor for the shortened url form.
 */
function gsb_url_shortener_form($form, &$form_state, \GSBUrlShortener $link) {
  $form['is_new'] = array(
    '#type' => 'value',
    '#value' => $link->isNew(),
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $link->id,
  );
  $form['alias'] = array(
    '#title' => t('Alias'),
    '#type' => 'textfield',
    '#description' => t('A unique link alias.'),
    '#field_prefix' => $GLOBALS['base_url'] . '/r/',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => $link->alias,
  );
  $form['destination'] = array(
    '#title' => t('Destination'),
    '#description' => t("An absolute URL, including 'http://'."),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#default_value' => $link->destination,
    //'#element_validate' => array('gsb_url_shortener_destination_validate'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('gsb_url_shortener_form_submit'),
    '#value' => t('Save shortened url'),
  );
  return $form;
}

/**
 * Form submission handler for gsb_url_shortener_form().
 */
function gsb_url_shortener_form_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['is_new'])) {
    drupal_write_record('gsb_url_shortener', $form_state['values']);
    drupal_set_message(t('Shortened url %alias has been updated', array('%alias' => $form_state['values']['alias'])));
  }
  else {
    drupal_write_record('gsb_url_shortener', $form_state['values'], array('id'));
    drupal_set_message(t('Shortened url %alias has been updated', array('%alias' => $form_state['values']['alias'])));
  }
  $form_state['redirect'] = 'admin/config/gsb/url-shortener';
}

/**
 * Callback for form_validate_machine_name().
 *
 * @param string $value
 *   The value of the element being validated.
 * @param array $element
 *   An associative array representing the element being validated.
 * @param array $form_state
 *   An associative array containing the current state of the form.
 *
 * @return bool
 *   TRUE if the link exists, FALSE otherwise.
 */
function gsb_url_shortener_id_exists($value, $element, $form_state) {
  return (bool) \GSBUrlShortener::loadById($value, $form_state['values']['type']);
}

/**
 * Form constructor for the shortened url delete form.
 */
function gsb_url_shortener_delete_form($form, &$form_state, \GSBUrlShortener $shortener) {
  $form_state['storage']['gsb_url_shortener'] = $shortener;
  return confirm_form($form, t('Are you sure you want to delete the shortened link %alias?', array('%alias' => $shortener->alias)), 'admin/config/gsb/url-shortener');
}

/**
 * Form submission handler for gsb_url_shortener_delete_form().
 */
function gsb_url_shortener_delete_form_submit(&$form, &$form_state) {
  $shortener = $form_state['storage']['gsb_url_shortener'];
  drupal_set_message(t('Shortened url %alias has been deleted', array('%alias' => $shortener->alias)));
  $shortener->delete();
  $form_state['redirect'] = 'admin/config/gsb/url-shortener';
}