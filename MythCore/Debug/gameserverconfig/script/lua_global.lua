
function Global_reloadFile(filePath)
	package.loaded[filePath] = nil				-- ���������¼
    require(filePath)							-- ���¼���lua�ļ�
end