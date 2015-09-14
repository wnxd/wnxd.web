﻿(function () { function Ready(fn) { if (document.addEventListener) { document.addEventListener("DOMContentLoaded", function () { document.removeEventListener("DOMContentLoaded", arguments.callee, false); fn() }, false) } else if (document.attachEvent) { document.attachEvent("onreadystatechange", function () { if (document.readyState === "complete") { document.detachEvent("onreadystatechange", arguments.callee); fn() } }) } } function BindEvent(dom, type, f) { if (dom.addEventListener) dom.addEventListener(type, f, false); else if (dom.attachEvent) dom.attachEvent(type, f) } function UnBindEvent(dom, type, f) { if (dom.addEventListener) dom.removeEventListener(type, f, false); else if (dom.attachEvent) dom.detachEvent(type, f) } function Ajax(type, url) { var XMLHttpReq, argv = arguments; try { XMLHttpReq = new ActiveXObject("Msxml2.XMLHTTP") } catch (e) { try { XMLHttpReq = new ActiveXObject("Microsoft.XMLHTTP") } catch (e) { XMLHttpReq = new XMLHttpRequest() } } type = type.toLowerCase(); XMLHttpReq.open(type, url, true); XMLHttpReq.onreadystatechange = function (e) { if (XMLHttpReq.readyState == 4) { var callback; if (type == "post") callback = argv[3]; else callback = argv[2]; if (typeof (callback) == "function") callback(XMLHttpReq.responseText, e) } }; var data = null; if (type == "post") { XMLHttpReq.setRequestHeader("Content-Type", "application/x-www-form-urlencoded"); data = argv[2] } XMLHttpReq.send(data) } Ready(function () { var list = []; for (var i = 0; i < document.all.length; i++) { var dom = document.all[i], wnxd_load = dom.getAttribute("wnxd-load"); if (wnxd_load != null && wnxd_load != "") list.push(dom) } if (list.length > 0) { var html = document.getElementsByTagName("html")[0], Refresh = function () { var i = 0, h = html.clientHeight; while (i < list.length) { var d = list[i], threshold = parseInt(d.getAttribute("wnxd-load")); if (d.getBoundingClientRect().top <= h + threshold) { list.splice(i, 1); (function (dom) { Ajax("post", "$url$", "wnxd_load=" + dom.getAttribute("id"), function (e) { var reg = /<script.*?>([\s\S]*)<\/script>/g, res; dom.outerHTML = e.replace(reg, ""); while ((res = reg.exec(e)) != null) eval(res[1]) }) })(d) } else { i++ } } if (list.length == 0) UnBindEvent(window, "scroll", Refresh) }; BindEvent(window, "scroll", Refresh); Refresh() } }) })();