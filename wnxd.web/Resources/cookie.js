(function () { var list = document.getElementsByTagName("script"), len = 0, callback; if (!window.wnxd) window.wnxd = {}; window.wnxd.SyncCookie = function (func) { if (typeof (func) == "function") { callback = func } }; if (list) { len = list.length - 1; for (var i = 0; i < len; i++) { var script = list[i]; script.onload = script.onreadystatechange = function () { var status = this.getAttribute("isload"); if (status == null) { if (!this.readyState || this.readyState === "loaded" || this.readyState === "complete") { this.setAttribute("isload", "0"); len--; if (len == 0 && callback) { callback(); callback = undefined } } } }; script.src = script.getAttribute("wnxd_cookie") } } function Ready(fn) { if (document.addEventListener) { document.addEventListener("DOMContentLoaded", function () { document.removeEventListener("DOMContentLoaded", arguments.callee, false); fn() }, false) } else if (document.attachEvent) { document.attachEvent("onreadystatechange", function () { if (document.readyState === "complete") { document.detachEvent("onreadystatechange", arguments.callee); fn() } }) } } Ready(function () { if (len == 0 && callback) { callback(); callback = undefined } }) })();