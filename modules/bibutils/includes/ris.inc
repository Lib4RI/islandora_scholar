<?php

/**
 * @file
 * Class for converting MODS to RIS
 *
 * @author Nelson Hart <nelson@discoverygarden.ca>
 * @date Sept 10, 2012 / https://github.com/Islandora/islandora_scholar/blob/7.x/modules/bibutils/includes/ris.inc
 *
 * @todo Build method for combining multiple nodes into one RIS field that is
 * not repeatable.
 * Eg: AB is not a repeatable field, but in my xml, I have multiple abstracts
 * from PUBMED. This new method would take all the nodes and combine them into
 * one string.
 */

class RIS {

  /**
   * Specifies all the different RIS types
   * Revised according to Redmine Issue #217: http://lib-dora-dev1.emp-eaw.ch:3000/issues/217
   *
   * @var array
   */
  protected $types = array(
    "abstract" => "ABST",
    "audiovisual material" => "ADVS",
    "aggregated database" => "AGGR",
    "ancient text" => "ANCIENT",
    "art work" => "ART",
    "bachelor thesis" => "THES",
    "bill" => "BILL",
    "blog" => "BLOG",
    "case" => "CASE",
    "book" => "BOOK",
    "book chapter" => "CHAP",
    "brochure" => "PAMP",
    "classical work" => "CLSWK",
    "computer program" => "COMP",
    "conference item" => "CPAPER",
    "conference paper" => "CPAPER",
    "conference proceeding" => "CONF",
    "conference proceedings" => "CONF",
    "catalog" => "CTLG",
    "data file" => "DATA",
    "dissertation" => "THES",
    "dictionary" => "DICT",
    "edited book" => "EDBOOK",
    "electronic book" => "EBOOK",
    "electronic book section" => "ECHAP",
    "electronic article" => "EJOUR",
    "encyclopedia" => "ENCYC",
    "equation" => "EQUA",
    "figure" => "FIGURE",
    "generic" => "GEN",
    "government document" => "GOVDOC",
    "grant" => "GRANT",
    "hearing" => "HEAR",
    "internet communication" => "ICOMM",
    "in press" => "INPR",
    "journal (full)" => "JFULL",
    "journal" => "JOUR",
    "journal article" => "JOUR",
    "legal rule or regulation" => "LEGAL",
    "manuscript" => "MANSCPT",
    "map" => "MAP",
    "magazine article" => "MGZN",
    "master thesis" => "THES",
    "motion picture" => "MPCT",
    "music score" => "MUSIC",
    "newspaper" => "MGZN",
    "newspaper/magazine article" => "MGZN",
    "newspaper or magazine article" => "MGZN",
    "online multimedia" => "MULTI",
    "online database" => "DBASE",
    "pamphlet" => "PAMP",
    "patent" => "PAT",
    "personal communication" => "PCOMM",
    "proceeding paper" => "CPAPER",
    "proceedings paper" => "CPAPER",
    "report" => "RPRT",
    "serial publication" => "SER",
    "slide" => "SLIDE",
    "sound recording" => "SOUND",
    "standard" => "STAND",
    "statute" => "STAT",
    "thesis/dissertation" => "THES",
    "thesis" => "THES",
    "unpublished work" => "UNPB",
    "video recording" => "VIDEO",
    "web page" => "ELEC",
    "whole book" => "BOOK",
  );

  /**
   * Specifies required RIS fields
   *
   * @var array
   */
  protected $requiredFields = array( "TY", "ER" );

  /**
   * Specifies which RIS fields are meant for names, in order to use renderName()
   *
   * @var array
   */
  protected $nameFields = array("A2", "A3", "A4", "AU", "ED" /* , "TA" */ );

  /**
   * Specifies which RIS fields are repeatable
   *
   * @var array
   */
  protected $repeatFields = array("A2", "A3", "A4", "AU", "KW", "ED");

  /**
   * Specifies which RIS fields are allowed
   *
   * @var array
   */
  protected $allowedFields = array(
    "TY" => "Type of reference (must be the first tag)",
    "A2" => "Secondary Author (each author on its own line preceded by the tag)",
    "A3" => "Tertiary Author (each author on its own line preceded by the tag)",
    "A4" => "Subsidiary Author (each author on its own line preceded by the tag)",
    "AB" => "Abstract",
    "AD" => "Author Address",
    "AN" => "Accession Number",
    "AU" => "Author (each author on its own line preceded by the tag)",
    "C1" => "Custom 1",
    "C2" => "Custom 2",
    "C3" => "Custom 3",
    "C4" => "Custom 4",
    "C5" => "Custom 5",
    "C6" => "Custom 6",
    "C7" => "Custom 7",
    "C8" => "Custom 8",
    "CA" => "Caption",
    "CN" => "Call Number",
    "CY" => "Place Published",
    "DA" => "Date",
    "DB" => "Name of Database",
    "DO" => "DOI",
    "DP" => "Database Provider",
    "EP" => "End Page",
    "ET" => "Edition",
    "IS" => "Number",
    "J2" => "Alternate Title (this field is used for the abbreviated title of a book or journal name)",
    "KW" => "Keywords (keywords should be entered each on its own line preceded by the tag)",
    "L1" => "File Attachments (this is a link to a local file on the users system not a URL link)",
    "L4" => "Figure (this is also meant to be a link to a local file on the users's system and not a URL link)",
    "LA" => "Language",
    "LB" => "Label",
    "M1" => "Number",
    "M3" => "Type of Work",
    "N1" => "Notes",
    "NV" => "Number of Volumes",
    "OP" => "Original Publication",
    "PB" => "Publisher",
    "PY" => "Year 20001/01// or 2011/02/01/ or 2011/02/01/<Other date info>",
    "RI" => "Reviewed Item",
    "RN" => "Research Notes",
    "RP" => "Reprint Edition",
    "SE" => "Section",
    "SN" => "ISBN/ISSN",
    "SP" => "Start Page",
    "ST" => "Short Title",
    "T2" => "series title Secondary Title",
    "T3" => "Tertiary Title",
    "TA" => "Translated Author",
    "TI" => "Title",
    "TT" => "Translated Title",
    "UR" => "URL",
    "VL" => "Volume",
    "Y2" => "Access Date",
    "ED" => "editor bibutils had this field",
    "ID" => "ID of object exported. I've noticed zotero and endnote add this unique ID field on their exports",
    "ER" => "End of Reference",
  );

  /**
   * Map MODS field (XPath) to RIS field
   *
   * @var array
   */
  protected $map;

  /**
   * The returned text of the class
   *
   * @var string
   */
  protected $text;

  /**
   * Create a ris instance with the xpath map for RIS fields.
   */
  protected function __construct($map) {
    $missing = FALSE;
    foreach ($this->requiredFields as $field) {
      if ( !isset($map[$field]) ) {
        $missing = TRUE;
      }
    }
    if ($missing == FALSE) {
      $this->map = $map;
    }
    else {
      drupal_set_message(t('Your RIS field map is missing required fields.'), 'error');
    }
  }

  /**
   * Gets an instance of this object.
   */
  protected static function getInstance() {
    static $instance = NULL;
    if ($instance === NULL) {
      module_load_include('inc', 'bibutils', 'includes/ris_map');
      $instance = new self(bibutils_ris_field_map());
    }
    return $instance;
  }

  /**
   * Convenience method to transform the given MODS record into RIS.
   */
  public static function transform($mods) {
    return self::getInstance()->render($mods);
  }

  /**
   * Convenience method to transform the given MODS record into RIS.
   */
  public static function transformFile($mods_file) {
    return self::getInstance()->render(file_get_contents($mods_file));
  }

  /**
   * Gets the type of RIS Document.
   *
   * Eg: Book (BOOK), Book Chapter (CHAP)
   *
   * @param string $type
   *   The type.
   *
   * @return string
   *   RIS document type
   */
  protected function getType($type) {
    $type = strtolower($type);
    return isset($this->types[$type]) ? $this->types[$type] : "STD";
  }

  /*
   * Function to render a date string in a RIS compliant way as good as possible.
   * 
   * See function getDate() below to get an overview what RIS compliance means.
   * It is tuned to deal with conference periods. Probable RIS compliant output may look like: "2019/03/12/ until /2019/04/15"
   */
  protected function getDateCustom($mods_date, $strict = 2, $format = "Y/m/d") {    // feb-2021/fh revised for flexibility+speed
    $mods_date = trim($mods_date,".:;/- \n\r\t\v\0");
    if ( intval($mods_date) && preg_match('/^[0-9]{4}$/',$mods_date) ) {
      return $mods_date;  // this is just the year
    }
    if ( $timeInt = strtotime($mods_date) ) {  // should also deal with e.g. '2019-03-15'
      return date($format,$timeInt);  // for "YYYY/MM/DD", see https://en.wikipedia.org/wiki/RIS_(file_format)
    }
    if ( count(explode('-',$mods_date)) != 2 ) {  // only stay here with *1* dash
      return $mods_date;  // bail out with any value - bad, but no concept for this
    }

    // Timespan care for "March 12-15th, 2019" or "March 12nd - April 27, 2019" or maybe even "2019, March 12-15th"
    $dateTmp = strtr($mods_date,".:;/,\t","      "); // so we just need to deal with space as separator
    $dateYear = '';
    if ( preg_match('/^[0-9]{4}\s+/', $dateTmp) ) {
      $dateYear = substr($dateTmp,0,4);
      $dateTmp = ltrim(substr($dateTmp,4),' ');
    } elseif ( preg_match('/\s+[0-9]{4}$/', $dateTmp)) {
      $dateYear = substr($dateTmp,-4);
      $dateTmp = rtrim(substr($dateTmp,0,-4),' ');
    } else {
      return $mods_date;  // bail out with any value - bad, but no concept for this
    }
    $spanAry = array('','');  // to hold start and end date
    if ( preg_match('/\w{3,}\s*\d{1,2}\w{0,2}\s*\-\s*\w{3,}\s*\d{1,2}\w{0,2}/',$dateTmp) ) { // to find "March 12nd - April 27"
      $spanAry[0] = rtrim(strtok($dateTmp,'-'),' ');
      $spanAry[1] = ltrim(strchr($dateTmp,'-'),' -');
    }
    elseif ( preg_match('/\w{3,}\s*\d{1,2}\w{0,2}\s*\-\s*\d{1,2}\w{0,2}/',$dateTmp) ) { // to find "March 12 - 15th"
      $spanAry[0] = rtrim(strtok($dateTmp,'-'),' ');
      $spanAry[1] = preg_replace('/\s*\d{1,2}\w{0,2}\s*\-\s*/',' ',$dateTmp); // removing "12 - " to get "March 15th"
    }
    foreach( $spanAry as $dIdx => $dTmp ) {
      if ( empty($dTmp) || !( $timeInt = strtotime($dTmp.', '.$dateYear) ) ) {
        return $mods_date;  // bail out with any value - bad, but no concept for this
      }
      $spanAry[$dIdx] = date($format,$timeInt);
    }
    return ( ( !intval($spanAry[0]) && $strict > 4 ) ? '' : ( $spanAry[0] . '/ until /' . $spanAry[1] ) );
  }

  /**
   * Get the RIS date format of a standard data format.
   *
   * @param string $mods_date
   *   Dates must be in the following format:   * YYYY/MM/DD/other info
   *
   *   The year, month and day fields are all numeric. The other info field can be any string of letters, spaces and hyphens.
   *
   *   Note that each specific date information is optional, however the slashes (“/”) are not.
   *   For example, if you just had the <year> and <other info>, then the output would look like: “1993///Spring.”
   *
   *   Date information should be set forth in the following format:
   *   YYYY or
   *   YYYY/MM or
   *   YYYY/MM/DD or
   *   YYYY/MM/DD/other info
   *
   * @return string
   *   RIS date
   */
  protected function getDate($mods_date, $strict = 1, $format = "Y/m/d") {		// mar-2021/fh revised for flexibility, relying on PHP's strtotime() + date() functions
    if ( $strict == 2 ) {
      return $this->getDateCustom($mods_date, $strict, $format);
    }
    $mods_date = trim($mods_date,".:;/- \n\r\t\v\0");
    if ( !intval($mods_date) || strpos(strtr($mods_date,'- /.:',',,,,,'),',') || !preg_match('/^[0-9]{4}$/',$mods_date) ) {
      // so $mods_date is not just a year number:
      $dateTmp = $mods_date;
      if ( $strict > 1 && strpos($dateTmp,'-') && strpos($dateTmp,',') ) { // try:
        $dateTmp = preg_replace('/\s*\-\s*\d{1,2}\w{0,2},/',',',$dateTmp);	// from "March 12-15th, 2019" just to "March 12, 2019"
        $dateTmp = preg_replace('/\s*\-\s*\w{3,}\s*\d{1,2}\w{0,2},/',',',$dateTmp);	// from "March 12 - April 27, 2019" just to "March 12, 2019"
      }
      if ( $timeInt = strtotime($dateTmp) ) {
        return date($format,$timeInt);	// for "YYYY/MM/DD", see https://en.wikipedia.org/wiki/RIS_(file_format)
      }
    }
    return ( ( !intval($mods_date) && $strict > 4 ) ? '' : $mods_date );
  }

  /**
   * Get the RIS Year.
   *
   * @param string $mods_date
   *   Format can be: YYYY*
   *
   * @return string
   *   RIS Year
   */
  protected function getYear($mods_date, $strict = 2, $format = "Y") {
    return $this->getDate($mods_date, $strict, $format);
  }

  /**
   * Get the name from MODS.
   *
   * @param SimpleXMLElement $names
   *   XML of the mods:name to the processed format:
   *   <name>
   *    <namePart type="family">
   *    <namePart type="given">
   *   </name>
   *
   * @param string $field
   *   RIS field to assiocate the name with
   */
  protected function renderName($names, $field) {
    if (!empty($field)) {
      $names->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
      $xTmp = $names->xpath(variable_get('islandora_scholar_xpaths_family', 'mods:namePart[@type = "family"]'));
      $family = trim($xTmp[0]);
      if ( !empty($family) ) { // there always should be a given name though
        $xTmp = $names->xpath(variable_get('islandora_scholar_xpaths_given', 'mods:namePart[@type = "given" ]'));
        $given = trim($xTmp[0]);
        // 2020-09-07/fh:
        // - Based on short tests: if there is no given name in our MODS then Endnote only supports/shows the *first* author in a nice way,
        //   further authors listed in Endnote will then however have an unneeded comma attached if they have no given name.
        // - If the given name is just a single letter, Endnote will automatically attach a dot/period if missing. Other tools may not.(?)
        // - After each 'author row' there is dot/period added (even if it's not an initial) - not really sure why, however
        //   Endnote at least seems to be able to handle it correctly.
        // Trying to tune this now... - keep TWO spaces in font of the dash!
        $this->text .= $field . "  - " . $family . ( @empty($given) ? '' : ', '.$given ) . ( @strlen($given) == 1 ? '.' : '' ) . "\n";
      }
    }
  }

  /**
   * Takes the given MODS XML and runs the RIS map XPaths against it.
   *
   * @param string $mods
   *   A string containing a MODS record (or a collection of them).
   *
   * @return string
   *   RIS formated text
   */
  protected function render($mods) { // revised ~2021-03-15/fh, trying to maintain orig. Islandora's code structure where feasible
    $this->text = "";
    if ( $risMapData = $this->map ) {
      $date_strict = variable_get('islandora_scholar_ris_date_strict',0);
      $mods_doc = new SimpleXMLElement($mods);
      $mods_doc->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');
      $mods_records = $mods_doc->xpath('//mods:mods');

      foreach ($mods_records as $mods_record) {
        $pubType = ''; // to carry an array value as in this->$types
        if ( @!empty($risMapData['TY']) ) { // there always should be a xPath for the TYpe though:
          $dataXml = $mods_record->xpath($risMapData['TY']);
          $pubType = !empty($dataXml[0]) ? $this->getType($dataXml[0]) : ( isset($dataXml[1]) ? $this->getType($dataXml[1]) : 'STD' );
        }
        foreach ($risMapData as $mapField => $xData) {
          if (array_key_exists($mapField, $this->allowedFields)) {
            if ($mapField == "TY") { // required field
              $this->text .= $mapField . "  - {$pubType}\n"; // keep TWO spaces in font of the dash!
              continue;
            }
            if ($mapField == "ER") { // required field
              $this->text .= $mapField . "  - \n\n\n"; // keep TWO spaces in font of the dash!
              continue;
            }

            $xPath = ''; // to be a string
            if ( !is_array($xData) ) {
              if ( !( $xPath = strval($xData) ) ) {
                continue;
              }
            } elseif ( !empty($pubType) && @!empty($xData[$pubType]) ) {
              $xPath = $xData[$pubType];
            } elseif ( @!empty($xData['_any']) ) {
              $xPath = $xData['_any'];
            } else {
              continue;
            }
            if ( !( $dataXml = $mods_record->xpath($xPath) ) || empty(reset($dataXml)) ) {
              if ( is_array($xData) && @!empty($xData['_aux']) ) {
                if ( !( $dataXml = $mods_record->xpath($xData['_aux']) ) ) {
                  continue;
                }
              }
            }

            foreach ($dataXml as $key => $val) {
              if (in_array($mapField, $this->nameFields)) {
                $this->renderName($dataXml[$key], $mapField); // will use/expand $this->text
              } else {
                $value = is_string($val) ? trim($val) : @trim($val[0]);
                if ($mapField == "PY") { // = publishing year
                  $value = rtrim( $this->getYear($value,$date_strict), '/:-,.');
                }
                elseif ($mapField == "DA" && $date_strict > 0) { // = date issued, option to block it (with 0) to respond on Jochen's phone call from 2021-04-13
                  $value = rtrim( $this->getDate($value,$date_strict), '/:-,.');
                }
                if (!empty($value)) {
                  $this->text .= $mapField . "  - $value\n"; // keep TWO spaces in font of the dash!
                }
              }
              if (!in_array($mapField, $this->repeatFields)) {
                break;
              }
            }
          }
        }
      }
    }
    return $this->text;
  }

}
