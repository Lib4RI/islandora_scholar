<?php

/**
 * @file
 * Defines an array to hold MODS XPaths for conveting to RIS
 *
 * @author Nelson Hart <nelson@discoverygarden.ca>
 * @date Sept 10, 2012
 */

/**
 * Gets the RIS field to MODS XPath mapping.
 *
 * @return array
 *   The RIS field to MODS XPath mapping.
 * 
 * Revised Feb-2021 for Lib4RI, see Excel file in Redmine Issue #217
 * 
 * Solr test query of currently stored values (here academic titles):
 * http://lib-dora-dev1.emp-eaw.ch:8080/solr/collection1/select?wt=csv&indent=true&csv.separator=%7c&sort=PID+asc&rows=987654321&q=PID:*%5C%3A*+AND+mods_note_academic%5C%20title_mt:*&fl=PID%2c+mods_note_academic?title_mt
 */
function bibutils_ris_field_map_feb21() {
  // XXX: Some comments go past 80 columns... Let's leave 'em be, for now.
  // @codingStandardsIgnoreStart
  return array(
    "TY" => "mods:genre", /* required field, actually the value of this tag will replaced by the RIS type finally */

    "TI" => "mods:titleInfo/mods:title",
    "AU" => "mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'author']",
    "PY" => "mods:originInfo/mods:dateIssued",

    "A2" => array(
      'CPAPER' => "mods:relatedItem[@type = 'host']/mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'editor']",
      '_aux'  => "mods:relatedItem[@type = 'host']/mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'editor']",
 	),
    "A3" => array(
      '_any' => "mods:relatedItem[@type = 'host']/mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'thesis advisor']",
      '_aux' => "mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'thesis advisor']",
 	),
 // "A4" => "Subsidiary Author (each author on its own line preceded by the tag)",
    "AB" => "mods:abstract[not(@displayLabel) or @displayLabel = 'combined-all-export']",
 // "AD" => "Author Address",
    "AN" => "mods:identifier[@type = 'accession']",
 // "C2" => "Custom 2",
 // "C3" => "Custom 3",
 // "C4" => "Custom 4",
 // "C5" => "Custom 5",
 // "C6" => "Custom 6",
 // "C7" => "Custom 7",
 // "C8" => "Custom 8",
 // "CA" => "Caption",
 // "CN" => "Call Number",
    "C1" => array(
      '_any' => "mods:relatedItem[@type = 'host']/mods:originInfo/mods:place/mods:placeTerm",	/* typical for 'CPAPER' */
    ),       
    "CY" => array(
      'CHAP' => "mods:relatedItem[@type = 'host']/mods:place/mods:placeTerm",
      'CONF' => "mods:originInfo[@eventType = 'conference']/mods:place/mods:placeTerm",
      '_aux' => "mods:originInfo/mods:place/mods:placeTerm",
    ),       
    "DA" => array(
      'CONF'  => "mods:originInfo[@eventType = 'conference']/mods:dateOther[@type = 'conferenceDate']",
      'CPAPER' => "mods:originInfo/mods:dateOther[@type = 'conferenceDate']",
 /*   '_aux'  => "mods:originInfo/mods:dateIssued",  */    /* see 'PY' tag which takes care about this */
    ),
 // "DB" => "Name of Database",
    "DO" => "mods:identifier[@type = 'doi']",
 // "DP" => "Database Provider",
    "ET" => "mods:originInfo/mods:edition",
    "IS" => "mods:relatedItem[@type = 'host']/mods:part/mods:detail[@type = 'issue']/mods:number",
    "J2" => "mods:relatedItem[@type = 'host']/mods:titleInfo[@type='abbreviated']/mods:title",
    "KW" => array(
      '_any' => "mods:subject/mods:topic",
      '_aux' => "mods:note[@type = 'keywords']",
	),
 // "L1" => "File Attachments (this is a link to a local file on the users system not a URL link)",
 // "L4" => "Figure (this is also meant to be a link to a local file on the users's system and not a URL link)",
    "LA" => "mods:language/mods:languageTerm",
 // "LB" => "Label",
    "M1" => "mods:relatedItem[@type = 'series']/mods:titleInfo/mods:partNumber", /* see also T3 */
    "M3" => array(
      'THES' => "mods:genre", /* only for THESis type, other publications types won't get this */
      '_aux' => "mods:note[@type = 'type of work']",
	),
 // "N1" => "Notes",
 // "NV" => "Number of Volumes",
 // "OP" => "Original Publication",
    "PB" => array(
      'THES' => "mods:name[@type = 'corporate' and mods:role/mods:roleTerm = 'Degree grantor']/mods:namePart",
      '_any' => "mods:relatedItem[@type = 'host']/mods:originInfo/mods:publisher",		/* typical for 'CHAP' + 'CPAPER' */
      '_aux' => "mods:originInfo/mods:publisher",
	),
    "RI" => "mods:note[@type = 'peer reviewed']",
 // "RN" => "Research Notes",
 // "RP" => "Reprint Edition",
 // "SE" => "Section",
    "SN" => "mods:identifier[@type = 'issn' or @type = 'isbn']",
    "SP" => array(		/* see https://www.wiki.lib4ri.ch/pages/viewpage.action?pageId=21889177 */
      'CPAPER' => "mods:relatedItem[@type = 'host']/mods:part/mods:extent[starts-with(@unit, 'page')]/mods:start",
      'CHAP' => "mods:relatedItem[@type = 'host']/mods:part/mods:extent[starts-with(@unit, 'page')]/mods:start",
      'JOUR' => "mods:relatedItem[@type = 'host']/mods:part/mods:extent[starts-with(@unit, 'page')]/mods:start",
      'MGZN' => "mods:relatedItem[@type = 'host']/mods:part/mods:extent[starts-with(@unit, 'page')]/mods:start",
      '_any' => "mods:physicalDescription/mods:extent[starts-with(@unit, 'page')]",
      '_aux' => "mods:relatedItem[@type = 'host']/mods:part/mods:extent[starts-with(@unit, 'page')]/mods:start",
	),
    "EP" => "mods:relatedItem[@type = 'host']/mods:part/mods:extent[starts-with(@unit, 'page')]/mods:end",
 // "ST" => "Short Title",
    "T2" => array(
      'CONF' => "mods:name[@type = 'conference']",
      '_aux' => "mods:relatedItem[@type = 'host']/mods:titleInfo[not(@type)]/mods:title",	/* typical for 'CPAPER' */
    ),
    "T3" => array(
      'CHAP' => "mods:relatedItem[@type = 'host']/mods:relatedItem[@type = 'series']/mods:titleInfo/mods:title",
      'CPAPER' => "mods:relatedItem[@type = 'host']/mods:relatedItem[@type = 'series']/mods:titleInfo[not(@type)]/mods:title",
      '_aux' => "mods:relatedItem[@type = 'series']/mods:titleInfo/mods:title",
	),
 // "TA" => "Translated Author",	/* depending on the xPath you may need to enable this also in ris.inc/$nameFields */
 // "TT" => "Translated Title",
    "UR" => "mods:identifier[@type = 'uri']",
    "VL" => "mods:relatedItem[@type='host']/mods:part/mods:detail[@type = 'volume']/mods:number",
    "SV" => array(
      'CHAP' => "mods:relatedItem[@type='host']/mods:relatedItem[@type = 'series']/mods:titleInfo/mods:partNumber",
      '_aux' => "mods:relatedItem[@type='host']/mods:part/mods:detail[@type = 'volume']/mods:number", /* typical for 'CPAPER' */
	),
 // "Y2" => "Access Date",
    "ED" => "mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'editor']",
    // ID added at export time for getting PID.
    "ID" => "mods:identifier[@type = 'id export']",

    "ER" => "",	/* Required field to mark the END of a data set of one publication */
  );
  // @codingStandardsIgnoreEnd
}



function bibutils_ris_field_map() {
	
  // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
  if ( function_exists('bibutils_ris_field_map_feb21') ) {	 // TEST/fh Feb 2021
    $roleList = trim( variable_get('lib4ri_ris_feb2021','repo manager, administrator') );
	if ( empty($roleList) || strchr('all|ok',$roleList) ) {
      return bibutils_ris_field_map_feb21();
    }
    global $user;
    foreach( explode(',',$roleList) as $role ) {
      if ( in_array(trim($role),array_values($user->roles)) ) {
		dpm( "Hello " . $user->name . " - Isn't RIS cool? So don't give it up!" );
        return bibutils_ris_field_map_feb21();
      }
	}
  }
  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

  // XXX: Some comments go past 80 columns... Let's leave 'em be, for now.
  // @codingStandardsIgnoreStart
  return array(
    "TY" => "mods:genre",
    "A2" => "mods:relatedItem[@type = 'host']/mods:name[@type = 'corporate' and mods:role/mods:roleTerm = 'author']/mods:namePart",
    // "A3" => "Tertiary Author (each author on its own line preceded by the tag)",
    // "A4" => "Subsidiary Author (each author on its own line preceded by the tag)",
    "AB" => "mods:abstract[not(@displayLabel) or @displayLabel = 'combined-all-export']",
    // "AD" => "Author Address",
    "AN" => "mods:identifier[@type = 'accession']",
    "AU" => "mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'author']",
    // "C1" => "Custom 1",
    // "C2" => "Custom 2",
    // "C3" => "Custom 3",
    // "C4" => "Custom 4",
    // "C5" => "Custom 5",
    // "C6" => "Custom 6",
    // "C7" => "Custom 7",
    // "C8" => "Custom 8",
    // "CA" => "Caption",
    // "CN" => "Call Number",
    "CY" => "mods:originInfo/mods:place/mods:placeTerm",
    // "DA" => "mods:originInfo/mods:dateIssued",
    // "DB" => "Name of Database",
    "DO" => "mods:identifier[@type = 'doi']",
    // "DP" => "Database Provider",
    "EP" => "mods:relatedItem[@type = 'host']/mods:part/mods:extent[@unit = 'page']/mods:end",
    "ET" => "mods:originInfo/mods:edition",
    "IS" => "mods:relatedItem[@type='host']/mods:part/mods:detail[@type = 'issue']/mods:number",
    "J2" => "mods:relatedItem[@type='host']/mods:titleInfo[@type='abbreviated']/mods:title",
    "KW" => "mods:note[@type = 'keywords']",
    // "L1" => "File Attachments (this is a link to a local file on the users system not a URL link)",
    // "L4" => "Figure (this is also meant to be a link to a local file on the users's system and not a URL link)",
    "LA" => "mods:language/mods:languageTerm",
    // "LB" => "Label",
    // "M1" => "Number",
    "M3" => "mods:note[@type = 'type of work']",
    // "N1" => "Notes",
    // "NV" => "Number of Volumes",
    // "OP" => "Original Publication",
    "PB" => "mods:originInfo/mods:publisher",
    "PY" => "mods:originInfo/mods:dateIssued",
    "RI" => "mods:note[@type = 'peer reviewed']",
    // "RN" => "Research Notes",
    // "RP" => "Reprint Edition",
    // "SE" => "Section",
    "SN" => "mods:identifier[@type = 'issn' or @type = 'isbn' ]",
    "SP" => "mods:relatedItem[@type='host']/mods:part/mods:extent[@unit = 'page']/mods:start",
    // "ST" => "Short Title",
    "T2" => "mods:relatedItem[@type = 'host']/mods:titleInfo[not(@type)]/mods:title",
    "T3" => "mods:relatedItem[@type = 'series']/mods:titleInfo/mods:title",
    // "TA" => "Translated Author",
    "TI" => "mods:titleInfo/mods:title",
    // "TT" => "Translated Title",
    // "UR" => "URL",
    "VL" => "mods:relatedItem[@type='host']/mods:part/mods:detail[@type = 'volume']/mods:number",
    // "Y2" => "Access Date",
    "ED" => "mods:name[@type = 'personal' and mods:role/mods:roleTerm = 'editor']",
    // ID added at export time for getting PID.
    "ID" => "mods:identifier[@type = 'id export']",
    "ER" => "",
  );
  // @codingStandardsIgnoreEnd
}
