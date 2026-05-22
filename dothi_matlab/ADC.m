clear; clc;

adc_300g  = [21, 21, 22, 21, 22, 21, 21, 22, 21, 21, 22, 21, 21, 22, 21, 22, 22, 22, 23, 22]; 
adc_600g  = [42, 42, 42, 43, 42, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43]; 
adc_900g  = [66, 66, 67, 66, 66, 66, 67, 67, 66, 68, 67, 67, 68, 68, 68, 67, 67, 66, 67, 67]; 
adc_1200g = [77, 78, 78, 78, 78, 78, 79, 79, 79, 79, 79, 79, 79, 79, 80, 79, 80, 80, 79, 80];
adc_1500g = [92, 92, 92, 91, 93, 93, 93, 92, 93, 93, 94, 93, 92, 92, 93, 92, 93, 93, 93, 92];

all_adc_data = [adc_300g, adc_600g, adc_900g, adc_1200g, adc_1500g];

len300  = length(adc_300g);
len600  = length(adc_600g);
len900  = length(adc_900g);
len1200 = length(adc_1200g);
len1500 = length(adc_1500g);

pos_300g  = len300 / 2;
pos_600g  = len300 + (len600 / 2);
pos_900g  = len300 + len600 + (len900 / 2);
pos_1200g = len300 + len600 + len900 + (len1200 / 2);
pos_1500g = len300 + len600 + len900 + len1200 + (len1500 / 2);

boundaries = [len300, len300+len600, len300+len600+len900, len300+len600+len900+len1200];

figure('Color', [1 1 1]);
hold on;

plot(all_adc_data, '-b', 'LineWidth', 2); 

for i = 1:length(boundaries)
    xline(boundaries(i), '--r', 'LineWidth', 1.2);
end

title('Đồ thị biến thiên ADC ', 'FontSize', 14);
ylabel('Giá trị số ADC', 'FontSize', 12);
grid on;

set(gca, 'XTick', [pos_300g, pos_600g, pos_900g, pos_1200g, pos_1500g]);
set(gca, 'XTickLabel', {'300g', '600g', '900g', '1200g', '1500g'}, 'FontSize', 11);

xlim([1, length(all_adc_data)]);
ylim([0, max(all_adc_data) + 15]);

hold off;