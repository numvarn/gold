/*Check action when user click*/

$(document).ready(function(){
	$("#transaction-reduce-form .pay-reduce-cost :input").number_format({precision: 0});

	var stake_cost = $("#edit-stake").val();
	var total_money = $("#edit-total-money").val();
	var inputs = $("#transaction-reduce-form .pay-reduce-cost :input");
	var values = {};
	var customer_must_pay = 0;
	var remain_money = 0;


	/*clean data*/
	stake_cost = Number(stake_cost.replace(/,/g,''));

	inputs.keyup(function(){
		var result = 0;
		var tmp = 0;

		$.each($('#transaction-reduce-form .pay-reduce-cost :input').serializeArray(), function(i, field) {
		    values[field.name] = field.value;
			tmp = field.value;
			tmp = tmp.replace(/,/g,'');

			result += Number(tmp);

			customer_must_pay = result + stake_cost;
		});

		remain_money = total_money - result;
		remain_money_html = $.number_format(remain_money, {precision: 0});
		customer_must_pay_html = $.number_format(customer_must_pay, {precision: 0});
		reduce_html = $.number_format(result, {precision: 0});

		$('div.remain-money').empty();
		$('div.remain-money').append(remain_money_html);

		$('td.pay-cost').empty();
		$('td.pay-cost').append(customer_must_pay_html);

		$("#edit-reduce-cost").empty();
		$("#edit-reduce-cost").val(result);

		$("td.cost").empty();
		$("td.cost").append(reduce_html);
	});

});
