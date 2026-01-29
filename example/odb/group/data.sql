
use `test_odb`;

insert into tb_classes values (1, 'class_A'), (2, 'class_B'), (3, 'class_C'), (4, 'class_D');
insert into tb_student (classes_id, name, age, score, birth) values
(1, 'Alice', 20, 85.5, '2003-01-15 10:00:00'),
(1, 'Bob', 21, 90.0, '2002-05-20 11:30:00'),
(2, 'Charlie', 19, 78.0, '2004-03-10 09:15:00'),
(2, 'David', 22, 88.5, '2001-07-25 14:45:00'),
(3, 'Eve', 20, 92.0, '2003-11-30 16:20:00'),
(3, 'Frank', 23, 81.0, '2000-12-05 08:50:00'),
(4, 'Grace', 21, 87.5, '2002-09-18 12:10:00'),
(4, 'Heidi', 19, 79.5, '2004-06-22 13:35:00');


