from mitmproxy import ctx, http, flowfilter  
  
class ReplaceSession:  
    def __init__(self):  
        self.flt = None  
        self.new_session_value = None  
      
    def load(self, loader):  
        loader.add_option(  
            "replace_session_filter",  
            str,  
            "",  
            "URL filter for session replacement (e.g., '~d example.com')"  
        )  
        loader.add_option(  
            "replace_session_value",  
            str,  
            "",  
            "New session value to replace"  
        )  
      
    def configure(self, updated):  
        if "replace_session_filter" in updated:  
            if ctx.options.replace_session_filter:  
                self.flt = flowfilter.parse(ctx.options.replace_session_filter)  
            else:  
                self.flt = None  
          
        if "replace_session_value" in updated:  
            self.new_session_value = ctx.options.replace_session_value  
      
    def request(self, flow: http.HTTPFlow):  
        if self.flt and self.new_session_value:  
            if flowfilter.match(self.flt, flow):  
                # 修改请求中的 session cookie  
                cookies = flow.request.cookies  
                if "session" in cookies:  
                    cookies["session"] = self.new_session_value  
  
addons = [ReplaceSession()]
