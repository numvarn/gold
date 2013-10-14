<?php
function report_conclusion() {
	global $user;
		
	/*get date*/
	list($day, $month, $year) = explode('-', $user->report_date); 
	list($day2, $month2, $year2) = explode('-', $user->report_date2); 
	
	/*set start and end date*/
	$min_time = mktime(0, 0, 0, $month, $day, $year); 
	$max_time = mktime(0, 0, 0, $month2, $day2, $year2); 
	
	$month_arr = report_get_month();
	$year_thai = $year + 543;
	$year_thai2 = $year2 + 543;
	
	$output = '';
	
	$pear_path = $_SERVER['DOCUMENT_ROOT']
			     .base_path()
				 .drupal_get_path("module", "report")
				 ."/PEAR";
				
	set_include_path(get_include_path() . PATH_SEPARATOR . $pear_path);
	
	require_once 'Spreadsheet/Excel/Writer.php';

	// Creating a workbook
	$workbook = new Spreadsheet_Excel_Writer();

	// sending HTTP headers
	$workbook->send('conclusion-ticket.xls');

	// Creating a worksheet
	$worksheet =& $workbook->addWorksheet('conclusion ticket');
	
	// Set align
	$format_center =& $workbook->addFormat();
	$format_center->setAlign('center');
	
	// top and center vertically
	$format_justify_center =& $workbook->addFormat();
	$format_justify_center->setVAlign('top');
	$format_justify_center->setAlign('center');
	
	// top vertically
	$format_v_center =& $workbook->addFormat();
	$format_v_center->setVAlign('top');
	
	/*header format*/
	$code_header =& $workbook->addFormat();
	$code_header->setVAlign('top'); 
	$code_header->setAlign('left');
	$code_header->setBold();
	$code_header->setBorderColor('blue');
	
	
	$row = 0;
	
	$worksheet->write($row, 0, report_conv("รายงานสรุปยอดเงินประจำเดือน ".$day." ".$month_arr[$month]." พ.ศ. ".$year_thai." ถึง ".$day2." ".$month_arr[$month2]." พ.ศ. ".$year_thai2));
	$worksheet->setMerge($row, 0, $row, 11);
	$row++;
	$row++;
	
	$conclusion_total = 0;
	
	/*Case 1*/ 
	$result = db_query("SELECT nid as nid, field_transac_withdraw_pay_value as withdraw 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transac_withdraw_item_date_value>=%d AND 
							  field_transac_withdraw_item_date_value<=%d AND  
							  field_transac_withdraw_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->total_cost;
	}
	
	$conclusion_total += $total_cost;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("รับจำนำ"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	
	$row++;
	
	/*Case 2*/ 
	$result = db_query("SELECT nid as nid, field_transaction_add_pay_value as add_cost 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
							  field_transaction_created_date_value<=%d AND  
							  field_transaction_add_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->add_cost;
	}
	
	$conclusion_total += $total_cost;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("ขอเพิ่มยอดจำนำ"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	
	$row++;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("รวม"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($conclusion_total))); $col++;
	
	$row++;
	$row++;
	
	/*Case 3*/ 
	$result = db_query("SELECT nid as nid, field_transaction_stake_pay_value as stake 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
							  field_transaction_created_date_value<=%d AND  
							  field_transaction_stake_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->stake;
	}
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("ดอกเบี้ย"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	
	$row++;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("รวม"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	
	$row++;
	$row++;
	
	/*Case 4*/ 
	$summation = 0;
	$result = db_query("SELECT nid as nid, field_transaction_close_pay_value as close 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_withdraw_date_value>=%d AND 
							  field_transaction_withdraw_date_value<=%d AND  
							  field_transaction_close_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->close;
	}
	
	$summation += $total_cost;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("ไถ่ถอน"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	$row++;
	
	$result = db_query("SELECT nid as nid, field_transac_withdraw_pay_value as withdraw 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
							  field_transaction_created_date_value<=%d AND  
							  field_transac_withdraw_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->withdraw;
	}
	
	$summation += $total_cost;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("ไถ่ถอนบางส่วน"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	
	$row++;
	
	$result = db_query("SELECT nid as nid, field_transaction_reduce_pay_value as reduce 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
							  field_transaction_created_date_value<=%d AND  
							  field_transaction_reduce_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->reduce;
	}
	
	$summation += $total_cost;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("ขอลดยอดเงินต้น"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	
	$row++;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("รวม"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($summation))); $col++;
	
	$row++;
	$row++;
	
	$result = db_query("SELECT nid as nid, ield_transaction_total_cost_value as total_cost  
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_expire_date_value>=%d AND 
							  field_transaction_expire_date_value<=%d AND  
							  field_transaction_status_value=%d", $min_time, $max_time, 6);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->total_cost;
	}
	
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("ขาดจำนำ"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	$row++;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv("รวม"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 30);
	$worksheet->write($row, $col, report_conv(number_format($total_cost))); $col++;
	$row++;
	
	// Let's send the file
	$workbook->close();
	
	return $output;
}

function report_conclusion_view() {
	global $user;
	
	report_set_breadcrumb();
	
	/*get date*/
	list($day, $month, $year) = explode('-', $user->report_date); 
	list($day2, $month2, $year2) = explode('-', $user->report_date2); 
	
	/*set start and end date*/
	$min_time = mktime(0, 0, 0, $month, $day, $year); 
	$max_time = mktime(0, 0, 0, $month2, $day2, $year2); 
	
	$month_arr = report_get_month();
	$year_thai = $year + 543;
	$year_thai2 = $year2 + 543;
	
	$output = '';
	
	
	$output .= '<div class="report-header">
					<div class="grid_1">'
						.t("รายงานสรุปยอดเงินประจำเดือน ".$day." ".$month_arr[$month]." พ.ศ. ".$year_thai). 
						' ถึง '
						.t($day2." ".$month_arr[$month2]." พ.ศ. ".$year_thai2).
			   '	</div>
					<div class="grid_2">'.l("ออกรายงาน Excel", "report/excel/conclusion").'</div>
				</div>
				<div class="clear-block"></div>';
					
	$conclusion_total = 0;
	
	/*Case 1*/ 
	$result = db_query("SELECT nid as nid, field_transaction_total_cost_value as total_cost 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
							  field_transaction_created_date_value<=%d AND  
							  field_transaction_old_ref_nid IS NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->total_cost;
	}
	
	$conclusion_total += $total_cost;
	
	$rows = array();
	
	$rows[] = array(
		array('class'=>'', 'data'=>'รับจำนำ'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	/*Case 2*/ 
	$result = db_query("SELECT nid as nid, field_transaction_add_pay_value as add_cost 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
							  field_transaction_created_date_value<=%d AND  
							  field_transaction_add_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->add_cost;
	}
	
	$conclusion_total += $total_cost;
	
	$rows[] = array(
		array('class'=>'', 'data'=>'ขอเพิ่มยอดจำนำ'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	$rows[] = array(
		array('class'=>'', 'data'=>'รวม'),
		array('class'=>'value', 'data'=>number_format($conclusion_total)),
	);
	
	/*Table 1*/ 
	$output .= '<div class="conclude-table">
					'.theme('table', $header, $rows).'
				</div>';
	
	/*Case 3*/ 
	$result = db_query("SELECT nid as nid, field_transaction_stake_pay_value as stake 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
							  field_transaction_created_date_value<=%d AND  
							  field_transaction_stake_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->stake;
	}
	
	$rows = array();
	
	$rows[] = array(
		array('class'=>'', 'data'=>'ดอกเบี้ย'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	$rows[] = array(
		array('class'=>'', 'data'=>'รวม'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	/*Table 2*/ 
	$output .= '<div class="conclude-table">
					'.theme('table', $header, $rows).'
				</div>';
	
	/*Case 4*/ 
	$summation = 0;
	$result = db_query("SELECT nid as nid, field_transaction_close_pay_value as close 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_withdraw_date_value>=%d AND 
							  field_transaction_withdraw_date_value<=%d AND  
							  field_transaction_close_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->close;
	}
	
	$summation += $total_cost;
	
	$rows = array();
	$rows[] = array(
		array('class'=>'', 'data'=>'ไถ่ถอน'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	$result = db_query("SELECT nid as nid, field_transac_withdraw_pay_value as withdraw 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transac_withdraw_item_date_value>=%d AND 
							  field_transac_withdraw_item_date_value<=%d AND  
							  field_transac_withdraw_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->withdraw;
	}
	
	$summation += $total_cost;
	
	$rows[] = array(
		array('class'=>'', 'data'=>'ไถ่ถอนบางส่วน'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	$result = db_query("SELECT nid as nid, field_transaction_reduce_pay_value as reduce 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transac_reduce_date_value>=%d AND 
							  field_transac_reduce_date_value<=%d AND  
							  field_transaction_reduce_pay_value IS NOT NULL", $min_time, $max_time);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->reduce;
	}
	
	$summation += $total_cost;
	
	$rows[] = array(
		array('class'=>'', 'data'=>'ขอลดยอดเงินต้น'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	$rows[] = array(
		array('class'=>'', 'data'=>'รวม'),
		array('class'=>'value', 'data'=>number_format($summation)),
	);
	
	/*Table 3*/ 
	$output .= '<div class="conclude-table">
					'.theme('table', $header, $rows).'
				</div>';
	

	$result = db_query("SELECT nid as nid, field_transaction_total_cost_value as total_cost  
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_expire_date_value>=%d AND 
							  field_transaction_expire_date_value<=%d AND  
							  field_transaction_status_value=%d", $min_time, $max_time, 6);
	
	$total_cost = 0;				
	while ($transaction = db_fetch_object($result)) {
		$total_cost += $transaction->total_cost;
	}
	
	$rows = array();
	$rows[] = array(
		array('class'=>'', 'data'=>'ขาดจำนำ'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	$rows[] = array(
		array('class'=>'', 'data'=>'รวม'),
		array('class'=>'value', 'data'=>number_format($total_cost)),
	);
	
	/*Table 4*/ 
	$output .= '<div class="conclude-table">
					'.theme('table', $header, $rows).'
				</div>';
	

	return $output;
}
?>