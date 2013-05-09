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
       ring_duration TIME DEFAULT '00:00:05'
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

