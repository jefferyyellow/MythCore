
DROP PROCEDURE IF EXISTS `checkusername`;
DELIMITER ;;
CREATE PROCEDURE `checkusername`(UserName char(32), ChannelID int, WorldID int)
BEGIN
	DECLARE AccountID INT UNSIGNED;
	SELECT account_id INTO AccountID FROM PlayerAccount where user_name=UserName and channel_id=ChannelID and world_id=WorldID;
	IF FOUND_ROWS() = 0 THEN
		INSERT INTO PlayerAccount(user_name,channel_id, world_id, create_time) VALUES(UserName, ChannelID, WorldID, unix_timestamp());
		SELECT LAST_INSERT_ID INTO AccountID;
		SELECT UserName, AccountID;
	ELSE
		SELECT UserName, AccountID;
	END IF;
END
DELIMITER ;