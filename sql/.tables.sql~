CREATE TABLE profiles(
       id INT NOT NULL AUTO_INCREMENT, 
       PRIMARY KEY(id),
       name VARCHAR(30)    
);
CREATE TABLE rings (
       id INT NOT NULL AUTO_INCREMENT, 
       PRIMARY KEY(id), 
       profile_id INT NOT NULL,
       ring_time TIME,
       ring_duration FLOAT DEFAULT '5.0',
       sub_ring BOOL DEFAULT '0'
);
CREATE TABLE rules (
       rule_day INT,
       PRIMARY KEY(rule_day),
       profile_id INT NOT NULL       
);

CREATE TABLE overrides(
       id INT NOT NULL AUTO_INCREMENT, 
       PRIMARY KEY(id), 
       over_date DATE,
       profile_id INT NOT NULL
);

CREATE TABLE sub_conf( 
	ring_before TIME DEFAULT '00:05:00', 
	ring_for FLOAT DEFAULT '0.5',
	global_enable BOOL DEFAULT 1
);
	
	

