ENTITY test1 IS
	PORT (x1, x2, x3 	: IN BIT;
				f						: OUT BIT);
END test1;

ARCHITECTURE LogicFunc OF test1 IS
BEGIN
	f <= (x1 AND x2) OR (NOT x2 AND x3);
END LogicFunc;

