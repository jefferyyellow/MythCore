
DROP PROCEDURE IF EXISTS `CheckUserName`;
DELIMITER ;;
CREATE PROCEDURE `CheckUserName`(UserName char(32), ChannelID int unsigned, ServerID int unsigned)
BEGIN
	DECLARE AccountID INT UNSIGNED;
	DECLARE RoleID INT UNSIGNED;

	SELECT account_id INTO AccountID FROM PlayerAccount where user_name=UserName and channel_id=ChannelID and server_id=ServerID;
	IF FOUND_ROWS() = 0 THEN
		INSERT INTO PlayerAccount(user_name,channel_id, server_id, create_time) VALUES(UserName, ChannelID, ServerID, unix_timestamp());
		SELECT LAST_INSERT_ID() INTO AccountID;
	END IF;

	SELECT role_id INTO RoleID from PlayerRole where account_id=AccountID and channel_id=ChannelID and server_id=ServerID;
	IF FOUND_ROWS() = 0 THEN
		SELECT AccountID, 0;
	ELSE
		SELECT AccountID, RoleID;
	END IF;
END
;;
DELIMITER ;

DROP PROCEDURE IF EXISTS `GetRoleInfo`;
DELIMITER ;;
CREATE PROCEDURE `GetRoleInfo`(AccountID int unsigned, ChannelID int unsigned, ServerID int unsigned)
BEGIN
	DECLARE RoleID INT UNSIGNED;
	SELECT role_id INTO RoleID from PlayerRole where account_id=AccountID and channel_id=ChannelID and server_id=ServerID;
	IF FOUND_ROWS() = 0 THEN
		SELECT 0;
	ELSE
		SELECT RoleID;
	END IF;
END
;;
DELIMITER ;


DROP PROCEDURE IF EXISTS `CreateRole`;
DELIMITER ;;
CREATE PROCEDURE `CreateRole`(RoleID int unsigned, RoleName char(32), AccountID int unsigned, ChannelID int unsigned, ServerID int unsigned)
BEGIN
	DECLARE tmpRoleID INT UNSIGNED;
	SELECT role_id from PlayerRole where account_id=AccountID and channel_id=ChannelID and server_id=ServerID;
	IF FOUND_ROWS() = 0 THEN
		
		INSERT INTO PlayerRole (role_id,role_name,account_id,channel_id,server_id) values(RoleID, RoleName, AccountID, ChannelID, ServerID);
		SELECT role_id INTO tmpRoleID from PlayerRole WHERE role_id=RoleID;
		IF FOUND_ROWS() = 0 THEN
			SELECT 0;
		ELSE
			SELECT tmpRoleID;
		END IF;
	ELSE
		SELECT 0;
	END IF;
END
;;
DELIMITER ;


DROP PROCEDURE IF EXISTS `LoadPlayerInfo`;
DELIMITER ;;
CREATE PROCEDURE `LoadPlayerInfo`(RoleID int unsigned)
BEGIN
	select role_name, role_level, role_exp from PlayerRole WHERE role_id=RoleID;
END
;;
DELIMITER ;