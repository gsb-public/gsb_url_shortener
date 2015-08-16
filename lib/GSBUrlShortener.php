<?php

/**
 * @file
 * Contains \GSBUrlShortener.
 */

/**
 * Class GSBUrlShortener.
 */
class GSBUrlShortener {

  /**
   * The unique id for the url.
   *
   * @var int
   */
  public $id;

  /**
   * The alias for the url.
   *
   * @var string
   */
  public $alias;

  /**
   * The destination url.
   *
   * @var string
   */
  public $destination;

  /**
   * Indicates if this link is new.
   *
   * @return bool
   */
  public function isNew() {
    return empty($this->id);
  }

  /**
   * Deletes the shortened link.
   */
  public function delete() {
    db_delete('gsb_url_shortener')
      ->condition('id', $this->id)
      ->execute();
  }

  /**
   * Loads a set of urls based on the current pager.
   *
   * @param array $header
   *   The header array for the paged table.
   *
   * @return \GSBUrlShortener[]
   */
  public static function loadFromPager($header) {
    $query = db_select('gsb_url_shortener', 'cl')
      ->extend('TableSort')
      ->extend('PagerDefault')
      ->fields('cl')
      ->limit(50)
      ->orderByHeader($header)
      ->execute();

    $return = array();
    $query->setFetchMode(PDO::FETCH_CLASS, 'GSBUrlShortener');
    foreach ($query as $record) {
      $return[] = $record;
    }
    return $return;
  }

  /**
   * Loads a given link by its id.
   *
   * @param string $id
   *   The shortened url id.
   *
   * @return \GSBUrlShortener|bool
   *   The link object, or FALSE if none exist with that id.
   */
  public static function loadById($id) {
    $query = db_select('gsb_url_shortener', 'cl')
      ->fields('cl')
      ->condition('id', $id)
      ->execute();

    return $query->fetchObject('GSBUrlShortener');
  }

  /**
   * Loads a given link by its alias.
   *
   * @param string $id
   *   The shortened url alias.
   *
   * @return \GSBUrlShortener|bool
   *   The link object, or FALSE if none exist with that alias.
   */
  public static function loadByAlias($alias) {
    $query = db_select('gsb_url_shortener', 'cl')
      ->fields('cl')
      ->condition('alias', $alias)
      ->execute();

    return $query->fetchObject('GSBUrlShortener');
  }

}
