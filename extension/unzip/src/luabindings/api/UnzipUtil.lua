
--------------------------------
-- @module UnzipUtil
-- @parent_module unzip

--------------------------------
-- 
-- @function [parent=#UnzipUtil] unzipOneFile 
-- @param self
-- @param #char pathInZip
-- @param #char targetPath
-- @param #bool needPassword
-- @param #char password
-- @param #int handler
        
--------------------------------
-- 
-- @function [parent=#UnzipUtil] unzipAllFile 
-- @param self
-- @param #char targetDir
-- @param #bool needPassword
-- @param #char password
-- @param #int handler
        
--------------------------------
-- 
-- @function [parent=#UnzipUtil] getAllFiles 
-- @param self
-- @param #int handler
        
--------------------------------
-- 
-- @function [parent=#UnzipUtil] create 
-- @param self
-- @param #char zipFile
-- @return UnzipUtil#UnzipUtil ret (return value: UnzipUtil)
        
--------------------------------
-- 
-- @function [parent=#UnzipUtil] createFromMem 
-- @param self
-- @param #int buffer
-- @param #bool copyBuffer
-- @return UnzipUtil#UnzipUtil ret (return value: UnzipUtil)
        
return nil
