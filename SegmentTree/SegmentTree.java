import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;

public class SegmentTree
{
    private ArrayList<Integer> data;
    private int n;

    public SegmentTree(List<Integer> arr) {
        n = arr.size();
        data = new ArrayList<Integer>(2 * n);

        for (int idx = 0; idx < n; idx++) {
            data.add(0);
        }
        for (int idx = 0; idx < n; idx++) {
            data.add(arr.get(idx));
        }
        for (int idx = n-1; idx > 0; idx--) {
            data.set(idx, Math.min(data.get(2*idx), data.get(2*idx+1)));
        }
    }

    public void update(int idx, int value) {
        idx += n;
        data.set(idx, value);
        
        while (idx > 1) {
            idx /= 2;
            data.set(idx, Math.min(data.get(2*idx), data.get(2*idx+1)));
        }
    }

    public int minimum(int left, int right) {
        left += n;
        right += n;
        int min = Integer.MAX_VALUE;

        while (left < right) {
            if ((left & 1) == 1) {
                min = Math.min(min, data.get(left));
                left++;
            }
            if ((right & 1) == 1) {
                right--;
                min = Math.min(min, data.get(right));
            }
            left >>= 1;
            right >>= 1;
        }
        
        return min;
    }

    public static void main(String[] args) {
        SegmentTree st = new SegmentTree(Arrays.asList(5, 2, 3, 1, 4));
        for (int i = 0; i < 5; i++) {
            System.out.println(st.minimum(i, i+1));
        }
        System.out.println(st.minimum(i, i+1));

        System.out.println(st.minimum(1, 4));
        st.update(3, 10);
        System.out.println(st.minimum(1, 4));
        System.out.println(st.minimum(0, 5));
        st.update(4, 0);
        System.out.println(st.minimum(1, 4));
        System.out.println(st.minimum(0, 5));
	}
}
