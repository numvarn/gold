/*Check action when user click*/

$(document).ready(function(){
	$("#transaction-add-cost-form .pay-reduce-cost :input").number_format({precision: 0});

	var stake_cost = $("#edit-stake").val();
	var total_cost = $("#edit-total-money-int").val();
	var inputs = $("#transaction-add-cost-form .pay-reduce-cost :input");
	var values = {};
	var customer_must_pay = 0;
	var remain_cost_int = 0;
	var remain_cost_html = "";

	/*clean data*/
	stake_cost = Number(stake_cost.replace(/,/g,''));

	inputs.keyup(function(){
		var result = 0;
		var tmp = 0;

		$.each($('#transaction-add-cost-form .pay-reduce-cost :input').serializeArray(), function(i, field) {
		    values[field.name] = field.value;
			tmp = field.value;
			tmp = tmp.replace(/,/g,'');

			result += Number(tmp);

			customer_must_pay = result - stake_cost;
		});

		remain_cost_int = parseInt(total_cost) + parseInt(result);

		customer_must_pay_html = $.number_format(customer_must_pay, {precision: 0});
		reduce_html = $.number_format(result, {precision: 0});
		remain_cost_html = $.number_format(remain_cost_int, {precision: 0});

		$('td.markup-total-money').empty();
		$('td.markup-total-money').append(remain_cost_html);

		$('td.pay-cost').empty();
		$('td.pay-cost').append(customer_must_pay_html);

		$("#edit-add-cost").empty();
		$("#edit-add-cost").val(result);

		$("td.cost").empty();
		$("td.cost").append(reduce_html);
	});

});
