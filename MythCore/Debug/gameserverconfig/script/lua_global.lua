
function Global_reloadFile(filePath)
	package.loaded[filePath] = nil				-- 消除载入记录
    require(filePath)							-- 重新加载lua文件
end