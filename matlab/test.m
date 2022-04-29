update_timer = timer('ExecutionMode', 'fixedRate', 'Period', 1/2, 'TimerFcn', @update_orientation);
start(update_timer);

function update_orientation(~, ~, ~)
    disp("Hello, world!");
end