
--------------------------------
-- @module HTTPRequest
-- @parent_module httpreq

--------------------------------
-- 
-- @function [parent=#HTTPRequest] getErrorCode 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] addPOSTValue 
-- @param self
-- @param #char key
-- @param #char value
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] addRequestHeader 
-- @param self
-- @param #char header
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] setReadTimeout 
-- @param self
-- @param #int timeout
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] setPOSTData 
-- @param self
-- @param #char data
-- @param #unsigned int len
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getResponseStatusCode 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getResponseDataLua 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getResponseString 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] setCookieString 
-- @param self
-- @param #char cookie
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getCookieString 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] start 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getResponseHeadersString 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getState 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getErrorMessage 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] saveResponseData 
-- @param self
-- @param #char filename
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] cancel 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] setConnectTimeout 
-- @param self
-- @param #int timeout
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] setTimeout 
-- @param self
-- @param #int timeout
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] getResponseDataLength 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#HTTPRequest] create 
-- @param self
-- @param #int listener
-- @param #char url
-- @param #char method
-- @return HTTPRequest#HTTPRequest ret (return value: HTTPRequest)
        
return nil
