%                                       total,  women,  white,    Black or \nAfrican American,  asian,  hispanic or latino

%Computer and mathematical occupations	5,352	25.8	65.7	8.7	23.1	7.8

%Architecture and engineering occupations	3,305	15.7	77.5	6.8	13.3	9.2

%Life, physical, and social science occupations	1,485	49.4	76.9	6.3	14.4	9.5

%Education, training, and library occupations	9,455	73.6	81.7	10.2	5.3	11.0

%Healthcare practitioners and technical occupations	9,684	75.4	75.3	12.5	9.6	9.0

cmTotal = 5352;
cmWomen = .258 * cmTotal;
cmWhite = .657 * cmTotal;
cmBlack = .087 * cmTotal;
cmAsian = .231 * cmTotal;
cmLatino = .078 * cmTotal;

subplot(2, 5, 1);
pie([cmWhite, cmBlack, cmAsian, cmLatino], {"White", "   Black or \nAfrican American", "Asian", "Hispanic or Latino"});
%axis("image")
title("Computer and Mathematical Occupations");
subplot(2, 5, 6);
pie([cmWomen, cmTotal - cmWomen], {"Women", "Other"});
%axis("image")

aeTotal = 3305;
aeWomen = .157 * aeTotal;
aeWhite = .775 * aeTotal;
aeBlack = .068 * aeTotal;
aeAsian = .133 * aeTotal;
aeLatino = .092 * aeTotal;

subplot(2, 5, 2);
pie([aeWhite, aeBlack, aeAsian, aeLatino], {"White", "   Black or \nAfrican American", "Asian", "Hispanic or Latino"});
%axis("image")
title("Architecture and Engineering Occupations");
subplot(2, 5, 7);
pie([aeWomen, aeTotal - aeWomen], {"Women", "Other"});
%axis("image")

lsTotal = 1485;
lsWomen = .494 * lsTotal;
lsWhite = .769 * lsTotal;
lsBlack = .063 * lsTotal;
lsAsian = .144 * lsTotal;
lsLatino = .095 * lsTotal;

subplot(2, 5, 3);
pie([lsWhite, lsBlack, lsAsian, lsLatino], {"White", "   Black or \nAfrican American", "Asian", "Hispanic or Latino"});
%axis("image")
title("Life, Physical, and Social Science Occupations");
subplot(2, 5, 8);
pie([lsWomen, lsTotal - lsWomen], {"Women", "Other"});
%axis("image")

edTotal = 9455;
edWomen = .736 * edTotal;
edWhite = .817 * edTotal;
edBlack = .102 * edTotal;
edAsian = .052 * edTotal;
edLatino = .11 * edTotal;

subplot(2, 5, 4);
pie([edWhite, edBlack, edAsian, edLatino], {"White", "   Black or \nAfrican American", "Asian", "Hispanic or Latino"});
%axis("image")
title("Education, Training, and Library Occupations");
subplot(2, 5, 9);
pie([edWomen, edTotal - edWomen], {"Women", "Other"});
%axis("image")

hcTotal = 9684;
hcWomen = .754 * hcTotal;
hcWhite = .753 * hcTotal;
hcBlack = .125 * hcTotal;
hcAsian = .096 * hcTotal;
hcLatino = .09 * hcTotal;

subplot(2, 5, 5);
pie([hcWhite, hcBlack, hcAsian, hcLatino], {"White", "   Black or \nAfrican American", "Asian", "Hispanic or Latino"});
%axis("image")
title("Healthcare Practitioners and Technical Occupations");
subplot(2, 5, 10);
pie([hcWomen, hcTotal - hcWomen], {"Women", "Other"});
%axis("image")

filename = "graph.png";
width = floor(1000);
width = 1080*2;
height = floor((2/5)*width);

size = strcat("-S", num2str(width), ",", num2str(height));
print(filename, "-dpng", size);
disp(strcat("Image Saved as ", " ", filename));
open(filename);