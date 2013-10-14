<?php
/**
 * function
 */
function report_reduce(){
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
	$workbook->send('reduce.xls');

	// Creating a worksheet
	$worksheet =& $workbook->addWorksheet('reduce');
	
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
	$code_header->setAlign('center');
	$code_header->setBold();
	$code_header->setBorderColor('blue');
	
	
	$row = 0;
	
	$worksheet->write($row, 0, report_conv("รายงานตั๋วจำนำขอลดยอดเงินต้นประจำเดือน ".$day." ".$month_arr[$month]." พ.ศ. ".$year_thai." ถึง ".$day2." ".$month_arr[$month2]." พ.ศ. ".$year_thai2));
	$worksheet->setMerge($row, 0, $row, 11);
	$row++;
	$row++;
	
	$col = 0;
	$worksheet->setColumn($col, $col, 8);
	$worksheet->write($row, $col, report_conv("หมายเลขตั๋ว"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 8);
	$worksheet->write($row, $col, report_conv("วันที่"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 8);
	$worksheet->write($row, $col, report_conv("จำนวนเงิน"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 10);
	$worksheet->write($row, $col, report_conv("ผู้ออกตั๋ว"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 10);
	$worksheet->write($row, $col, report_conv("อ้างอิงจากตั๋ว"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 8);
	$worksheet->write($row, $col, report_conv("วันที่"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 8);
	$worksheet->write($row, $col, report_conv("จำนวนเงิน"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 10);
	$worksheet->write($row, $col, report_conv("ผู้ออกตั๋ว"), $code_header); $col++;
	
	$worksheet->setColumn($col, $col, 10);
	$worksheet->write($row, $col, report_conv("ชื่อลูกค้า"), $code_header); $col++;
	
	$row++;
	
	
	/*query data*/ 
	$result = db_query("SELECT 	nid as nid,
								field_transaction_code_value as code,
		  					    field_transaction_created_date_value as create_date,
								field_transaction_total_cost_value as total_cost, 
								field_transaction_reduce_pay_value as reduce,
								field_transaction_add_pay_value as add_cost,
								field_transac_withdraw_pay_value as withdraw,
								field_customer_ref_nid as customer, 
								field_transaction_withdraw_item_value as items, 
								field_employee_term_value as emp 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
					          field_transaction_created_date_value<=%d AND 
							  field_transaction_reduce_pay_value IS NOT NULL", $min_time, $max_time);
	
	
	while ($transaction = db_fetch_object($result)) {
		$create_date = thai_time_format($transaction->create_date, 'd-F-Y', 1);
		
		$remain_cost = $transaction->total_cost;
		$remain_cost = number_format($remain_cost);
		
		/*get customer name*/ 
		$customer_name = db_result(db_query("SELECT title FROM {node} WHERE nid=%d", $transaction->customer));
		
		/*get emp name*/ 
		$uid = db_result(db_query("SELECT uid FROM {node} WHERE nid=%d", $transaction->nid));
		$emp_name = db_result(db_query("SELECT title FROM {node} WHERE uid=%d AND type='%s'", $uid, 'employee_profile'));
		
		/*get renewer name*/
		$result2 = db_query("SELECT nid 
							 FROM {content_type_transaction_gold} 
							 WHERE field_transaction_old_ref_nid=%d AND field_transaction_status_value!=%d LIMIT 1", $transaction->nid, 7); 
		$new_nid = db_result($result2);
		$uid = db_result(db_query("SELECT uid FROM {node} WHERE nid=%d", $new_nid));
		$renewer_name = db_result(db_query("SELECT title FROM {node} WHERE uid=%d AND type='%s'", $uid, 'employee_profile'));
		
		
		/*query data*/ 
		$result_new = db_query("SELECT nid as nid,
									field_transaction_code_value as code,
			  					    field_transaction_created_date_value as create_date,
									field_transaction_total_cost_value as total_cost, 
									field_transaction_reduce_pay_value as reduce,
									field_transaction_add_pay_value as add_cost,
									field_transac_withdraw_pay_value as withdraw,
									field_customer_ref_nid as customer, 
									field_transaction_withdraw_item_value as items, 
									field_employee_term_value as emp 
						     FROM {content_type_transaction_gold} 
						     WHERE field_transaction_old_ref_nid=%d AND field_transaction_status_value", $transaction->nid, 5);
		
		$transaction_new = db_fetch_object($result_new);
		
		$create_date_new = thai_time_format($transaction_new->create_date, 'd-F-Y', 1);
		
		$uid_new = db_result(db_query("SELECT uid FROM {node} WHERE nid=%d", $transaction_new->nid));
		$emp_name_new = db_result(db_query("SELECT title FROM {node} WHERE uid=%d AND type='%s'", $uid_new, 'employee_profile'));
		
		$worksheet->write($row, 0, report_conv($transaction_new->code), $format_justify_center);
		$worksheet->write($row, 1, report_conv($create_date_new), $format_justify_center);
	
		$worksheet->write($row, 2, report_conv(number_format($transaction_new->total_cost)), $format_justify_center);
		$worksheet->write($row, 3, report_conv($emp_name_new), $format_v_center);
		
		
		$worksheet->write($row, 4, report_conv($transaction->code), $format_justify_center);
		$worksheet->write($row, 5, report_conv($create_date), $format_justify_center);
		
		$worksheet->write($row, 6, report_conv(number_format($transaction->total_cost)), $format_justify_center);
		$worksheet->write($row, 7, report_conv($emp_name), $format_v_center);
		$worksheet->write($row, 8, report_conv($customer_name), $format_v_center);
		
		$row++;

	}
	
	// Let's send the file
	$workbook->close();
	
	return $output;
}

function report_reduce_view(){
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
						.t("รายงานตั๋วจำนำขอลดยอดเงินต้นประจำเดือน ".$day." ".$month_arr[$month]." พ.ศ. ".$year_thai). 
						' ถึง '
						.t($day2." ".$month_arr[$month2]." พ.ศ. ".$year_thai2).
			   '	</div>
					<div class="grid_2">'.l("ออกรายงาน Excel", "report/excel/reduce").'</div>
				</div>
				<div class="clear-block"></div>';
	$header = array(
		t("หมายเลขตั๋ว"),
		t("วันที่"),
		t("จำนวนเงิน"),
		t("ผู้ออกตั๋ว"),
		t("อ้างอิงจากตั๋ว"),
		t("วันที่"),
		t("จำนวนเงิน"),
		t("ผู้ออกตั๋ว"),
		t("ชื่อลูกค้า"),
	);
		
	/*query data*/ 
	$result = db_query("SELECT 	nid as nid,
								field_transaction_code_value as code,
		  					    field_transaction_created_date_value as create_date,
								field_transaction_total_cost_value as total_cost, 
								field_transaction_reduce_pay_value as reduce,
								field_transaction_add_pay_value as add_cost,
								field_transac_withdraw_pay_value as withdraw,
								field_customer_ref_nid as customer, 
								field_transaction_withdraw_item_value as items, 
								field_employee_term_value as emp 
					    FROM {content_type_transaction_gold} 
					    WHERE field_transaction_created_date_value>=%d AND 
					          field_transaction_created_date_value<=%d AND 
							  field_transaction_reduce_pay_value IS NOT NULL 
						ORDER BY field_transaction_code_value ASC", $min_time, $max_time);
	
	
	while ($transaction = db_fetch_object($result)) {
		$create_date = thai_time_format($transaction->create_date, 'd-F-Y', 1);
		
		$remain_cost = $transaction->total_cost;
		$remain_cost = number_format($remain_cost);
		
		/*get customer name*/ 
		$customer_name = db_result(db_query("SELECT title FROM {node} WHERE nid=%d", $transaction->customer));
		
		/*get emp name*/ 
		$uid = db_result(db_query("SELECT uid FROM {node} WHERE nid=%d", $transaction->nid));
		$emp_name = db_result(db_query("SELECT title FROM {node} WHERE uid=%d AND type='%s'", $uid, 'employee_profile'));
		
		/*get renewer name*/
		$result2 = db_query("SELECT nid 
							 FROM {content_type_transaction_gold} 
							 WHERE field_transaction_old_ref_nid=%d AND field_transaction_status_value!=%d LIMIT 1", $transaction->nid, 7); 
		$new_nid = db_result($result2);
		$uid = db_result(db_query("SELECT uid FROM {node} WHERE nid=%d", $new_nid));
		$renewer_name = db_result(db_query("SELECT title FROM {node} WHERE uid=%d AND type='%s'", $uid, 'employee_profile'));
		
		
		/*query data*/ 
		$result_new = db_query("SELECT nid as nid,
									field_transaction_code_value as code,
			  					    field_transaction_created_date_value as create_date,
									field_transaction_total_cost_value as total_cost, 
									field_transaction_reduce_pay_value as reduce,
									field_transaction_add_pay_value as add_cost,
									field_transac_withdraw_pay_value as withdraw,
									field_customer_ref_nid as customer, 
									field_transaction_withdraw_item_value as items, 
									field_employee_term_value as emp 
						     FROM {content_type_transaction_gold} 
						     WHERE field_transaction_old_ref_nid=%d AND field_transaction_status_value", $transaction->nid, 5);
		
		$transaction_new = db_fetch_object($result_new);
		
		$create_date_new = thai_time_format($transaction_new->create_date, 'd-F-Y', 1);
		
		$uid_new = db_result(db_query("SELECT uid FROM {node} WHERE nid=%d", $transaction_new->nid));
		$emp_name_new = db_result(db_query("SELECT title FROM {node} WHERE uid=%d AND type='%s'", $uid_new, 'employee_profile'));
		
		$tmp[] = array('class'=>'', 'data'=>$transaction_new->code);
		$tmp[] = array('class'=>'', 'data'=>$create_date_new);
		$tmp[] = array('class'=>'', 'data'=>number_format($transaction_new->total_cost));
		$tmp[] = array('class'=>'', 'data'=>$emp_name_new);
		$tmp[] = array('class'=>'', 'data'=>$transaction->code);
		$tmp[] = array('class'=>'', 'data'=>$create_date);
		$tmp[] = array('class'=>'', 'data'=>number_format($transaction->total_cost));
		$tmp[] = array('class'=>'', 'data'=>$emp_name);
		$tmp[] = array('class'=>'', 'data'=>$customer_name);
		
		$rows[] = $tmp;
		unset($tmp);
	}
	
	$output .= '<div class="">
					'.theme('table', $header, $rows).'
				</div>';
	
	return $output;
}
?>