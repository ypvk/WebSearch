var page = require("webpage").create(),
	sys = require("system")
	url = "http://www.baidu.com",
	kw = "test";

function printArgs() {
    var i, ilen;
    for (i = 0, ilen = arguments.length; i < ilen; ++i) {
        console.log("    arguments[" + i + "] = " + JSON.stringify(arguments[i]));
    }
    console.log("");
}

page.onUrlChanged = function() {
    console.log("page.onUrlChanged");
    printArgs.apply(this, arguments);
	loadFunc(arguments[0])
};

var loadFunc = function(_page, url) {
	_page.open(url, function(status) {
		console.log("status:" + status);
		if (status !== "success") {
			console.log("load url " + url + " failed")
		}
		_page.render("baidu.png")
		page.evaluate(function() {
			$('#kw').val("123");
			//$('#su').trigger("click");
			$("form").submit();
		});
		page.render("baidu1.png")
	});
}
loadFunc(page, url);
setTimeout(function() {
	phantom.exit(0);
}, 10000)
