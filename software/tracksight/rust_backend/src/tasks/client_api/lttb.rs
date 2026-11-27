
/**
 * Mostly used in signal_tile.rs just to perform downsampling on tiles
 * Did not like any of the existing libraries with LTTB
 */

/**
 * Can pass any generic struct type, so long as it identifies the x and y values
 */
pub trait LttbPoint {
    fn x(&self) -> f64;
    fn y(&self) -> f64;
}

pub fn lttb<T: LttbPoint>(
    data: &[T],
    threshold: usize,
) -> Vec<&T> {
    let n = data.len();

    if n == 0 || threshold == 0 {
        return Vec::new();
    }

    if n <= threshold {
        return data.iter().collect();
    }

    if threshold == 1 {
        return vec![&data[0]];
    }

    if threshold == 2 {
        return vec![&data[0], &data[n - 1]];
    }

    // requires at least 3 points to do lttb
    // keep the first and last points, selecting one point
    // from each intermediate bucket.
    let mut sampled = Vec::with_capacity(threshold);

    // Always preserve the first point.
    sampled.push(&data[0]);

    // Number of points in each bucket, excluding endpoints.
    let bucket_size =
        (n - 2) as f64 / (threshold - 2) as f64;

    let mut a = 0usize;

    for i in 0..(threshold - 2) {
        // current candidate bucket
        let range_start =
            (i as f64 * bucket_size).floor() as usize + 1;

        let range_end =
            ((i + 1) as f64 * bucket_size).floor() as usize + 1;

        // Next bucket, used to calculate its average.
        let avg_range_start =
            ((i + 1) as f64 * bucket_size).floor() as usize + 1;

        let avg_range_end =
            ((i + 2) as f64 * bucket_size).floor() as usize + 1;

        let avg_range_start = avg_range_start.min(n - 1);
        let avg_range_end = avg_range_end.min(n);

        // Calculate average point of the next bucket.
        let avg_range = &data[avg_range_start..avg_range_end];

        let (avg_x, avg_y) = if avg_range.is_empty() {
            // Fallback to the final point.
            (data[n - 1].x(), data[n - 1].y())
        } else {
            let count = avg_range.len() as f64;

            let sum_x: f64 =
                avg_range.iter().map(|p| p.x()).sum();

            let sum_y: f64 =
                avg_range.iter().map(|p| p.y()).sum();

            (sum_x / count, sum_y / count)
        };

        let point_a = &data[a];

        let ax = point_a.x();
        let ay = point_a.y();

        let mut max_area = -1.0;
        let mut max_area_index = range_start;

        let range_end = range_end.min(n - 1);

        // Select the candidate that forms the largest triangle.
        for j in range_start..range_end {
            let point = &data[j];

            let bx = point.x();
            let by = point.y();

            let area = triangle_area(
                ax, ay,
                bx, by,
                avg_x, avg_y,
            );

            if area > max_area {
                max_area = area;
                max_area_index = j;
            }
        }

        a = max_area_index;
        sampled.push(&data[a]);
    }

    // Always preserve the last point.
    sampled.push(&data[n - 1]);

    sampled
}

fn triangle_area(
    ax: f64,
    ay: f64,
    bx: f64,
    by: f64,
    cx: f64,
    cy: f64,
) -> f64 {
    ((ax - cx) * (by - ay)
        - (ax - bx) * (cy - ay))
        .abs()
        * 0.5
}