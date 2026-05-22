clear; clc;


delta_300g  = [5.99, 5.99, 5.99, 5.99, 5.99, 5.99, 5.99, 5.99, 5.99, 5.99, 6.10, 6.01, 5.99, 5.99, 5.99, 6.14, 6.14, 5.99, 6.33, 6.22]; 
delta_600g  = [12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98, 12.98]; 
delta_900g  = [20.98, 20.97, 20.98, 20.98, 20.98, 20.98, 20.98, 20.98, 20.98, 20.98, 20.98, 20.98, 20.98, 21.09, 20.98, 21.01, 21.00, 20.98, 21.03, 21.02]; 
delta_1200g = [25.28, 25.10, 25.10, 25.33, 25.76, 25.78, 25.98, 25.99, 26.00, 25.92, 25.96, 25.85, 25.95, 26.00, 25.98, 25.90, 25.98, 25.98, 25.99, 26.00];
delta_1500g = [31.58, 31.52, 31.67, 31.53, 31.91, 31.85, 31.94, 31.84, 31.96, 31.96, 31.95, 31.98, 31.98, 31.98, 31.98, 31.98, 31.98, 31.98, 31.98, 31.98];


all_delta_data = [delta_300g, delta_600g, delta_900g, delta_1200g, delta_1500g];

len300  = length(delta_300g);
len600  = length(delta_600g);
len900  = length(delta_900g);
len1200 = length(delta_1200g);
len1500 = length(delta_1500g);


pos_300g  = len300 / 2;
pos_600g  = len300 + (len600 / 2);
pos_900g  = len300 + len600 + (len900 / 2);
pos_1200g = len300 + len600 + len900 + (len1200 / 2);
pos_1500g = len300 + len600 + len900 + len1200 + (len1500 / 2);


boundaries = [len300, len300+len600, len300+len600+len900, len300+len600+len900+len1200];


figure('Color', [1 1 1]);
hold on;


plot(all_delta_data, '-b', 'LineWidth', 2); 

for i = 1:length(boundaries)
    xline(boundaries(i), '--r', 'LineWidth', 1.2);
end

title('Đồ thị biến thiên giá trị delta của RC Timer ', 'FontSize', 14);
ylabel('Giá trị delta', 'FontSize', 12);
grid on;

ytickformat('%d \\mus');

set(gca, 'XTick', [pos_300g, pos_600g, pos_900g, pos_1200g, pos_1500g]);
set(gca, 'XTickLabel', {'300g', '600g', '900g', '1200g', '1500g'}, 'FontSize', 11);

xlim([1, length(all_delta_data)]);
ylim([0, max(all_delta_data) + 15]);

hold off;